#include "commands/CmdJoin.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <map>

static bool parseChannels(const std::string &params, std::vector<std::string> &channelNames)
{
    std::istringstream iss(params);
    std::string value;
    while (std::getline(iss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            channelNames.push_back(value);
        else if (!value.empty())
            return false;
    }
    return true;
}

static void parseKeys(const std::string &params, std::vector<std::string> &keys)
{
    if (params.empty())
        return;
    std::istringstream iss(params);
    std::string value;
    while (std::getline(iss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty())
            keys.push_back(value);
    }
    return;
}

/**
 * @brief Realiza el proceso de unirse/crear a un canal concreto y envía los replies correspondientes.
 * @param ctx Contexto de la petición.
 * @param channelName Nombre del canal objetivo.
 * @param key Clave del canal (puede estar vacía).
 */
static void joinSingleChannel(RequestContext &ctx, const std::string &channelName, const std::string &key)
{
    Channel *channel = ctx.services.channels().getChannel(channelName);

    if (!channel) {
        try {
            ctx.services.channels().createChannel(channelName, ctx.sender);
        } catch (const std::exception &e) {
            ctx.services.sendToUser(ctx.sender, std::string("Failed to create channel: ") + channelName + " - " + e.what());
            return;
        }
        channel = ctx.services.channels().getChannel(channelName);
        if (!channel) {
            ctx.services.sendToUser(ctx.sender, std::string("Failed to obtain channel after creation: ") + channelName);
            return;
        }
    }
    else {
        try {
            ctx.sender->joinChannel(channel, key);
        } catch (const std::exception &e) {
            ctx.services.sendToUser(ctx.sender, std::string("Cannot join channel ") + channelName + ": " + e.what());
            return;
        }
    }
}

/**
 * @brief Envía el mensaje JOIN al canal (prefijo del usuario que se une).
 */
static void sendJoinMessage(RequestContext &ctx, Channel *channel)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) std::strcpy(hostname, "localhost");
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string joinMsg = ":" + ctx.sender->getNickname() + "!" + uname + "@" + hostname + " JOIN " + channel->getName() + "\r\n";
    ctx.services.sendToChannel(channel, joinMsg, NULL);
}

/**
 * @brief Construye y envía la lista de nombres del canal al usuario que se une.
 */
static void sendNamesList(RequestContext &ctx, Channel *channel)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) std::strcpy(hostname, "localhost");
    const std::map<int, User*>& usersMap = channel->getUsers();
    std::string namesList;
    for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
        if (!namesList.empty()) namesList += " ";
        if (channel->isUserOperator(uit->second))
            namesList += "@" + uit->second->getNickname();
        else
            namesList += uit->second->getNickname();
    }
    std::string namesReply = std::string(":") + hostname + " 353 " + ctx.sender->getNickname() + " = " + channel->getName() + " :" + namesList + "\r\n";
    ctx.services.sendToUser(ctx.sender, namesReply);

    std::string endNames = std::string(":") + hostname + " 366 " + ctx.sender->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
    ctx.services.sendToUser(ctx.sender, endNames);
}

// After extracting helpers we call them from the loop in execute()

void CmdJoin::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = ctx.rawLine;
    std::string keysPart;
    size_t sp = ctx.rawLine.find(' ');
    if (sp != std::string::npos) {
        channelsPart = ctx.rawLine.substr(0, sp);
        keysPart = ctx.rawLine.substr(sp + 1);
    }
    if (!parseChannels(channelsPart, channelNames)) {
        ctx.services.sendToUser(ctx.sender, std::string("Invalid channel name(s)"));
        return;
    }

    parseKeys(keysPart, keys);

    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        const std::string &channelName = channelNames[i];
        const std::string key = (i < keys.size() ? keys[i] : "");
        Channel *channel = ctx.services.channels().getChannel(channelName);
        joinSingleChannel(ctx, channelName, key);
        if (channel) {
            sendJoinMessage(ctx, channel);
            sendNamesList(ctx, channel);
        }
    }
}
