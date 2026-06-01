#include "commands/CmdJoin.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
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
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
            value.erase(value.size() - 1, 1);
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
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
            value.erase(value.size() - 1, 1);
        if (!value.empty())
            keys.push_back(value);
    }
    return;
}

// Límite por defecto de canales por usuario (si no hay configuración explícita)
static const size_t MAX_CHANNELS_PER_USER = 10;

static void sendResponse(RequestContext &ctx, const std::string &code, const std::string &message)
{
    std::string serverName = ctx.services.getServerName();
    std::string response = std::string(":") + serverName + " " + code + " " + ctx.sender->getNickname() + " " + message;
    ctx.services.sendToUser(ctx.sender, response);
}

static void joinSingleChannel(RequestContext &ctx, const std::string &channelName, const std::string &key)
{
    if (!ctx.sender) return;

    // Comprobar límite máximo de canales del usuario
    if (ctx.sender->getChannels().size() >= MAX_CHANNELS_PER_USER) {
        sendResponse(ctx, "405", channelName + " :You have joined too many channels");
        return;
    }

    Channel *channel = ctx.services.channels().getChannel(channelName);

    if (!channel) {
        try {
            ctx.services.channels().createChannel(channelName, ctx.sender);
        } catch (const IrcException &ie) {
            // Mapear códigos internos a numerics cuando sea posible
            int code = ie.getCode();
            if (code == IRC_ERR_CHANNEL_FULL)
                sendResponse(ctx, "471", channelName + " :Cannot join channel (+l)");
            else if (code == IRC_ERR_CHANNEL_INVITE_ONLY)
                sendResponse(ctx, "473", channelName + " :Cannot join channel (+i)");
            else if (code == IRC_ERR_USER_BANNED)
                sendResponse(ctx, "474", channelName + " :Cannot join channel (+b)");
            else if (code == IRC_ERR_INCORRECT_CHANNEL_KEY)
                sendResponse(ctx, "475", channelName + " :Cannot join channel (+k)");
            else
                sendResponse(ctx, "475", channelName + " :Cannot join channel");
            return;
        } catch (const std::exception &e) {
            sendResponse(ctx, "475", channelName + " :Cannot create/join channel");
            return;
        }
        channel = ctx.services.channels().getChannel(channelName);
        if (!channel) {
            sendResponse(ctx, "475", channelName + " :Failed to obtain channel after creation");
            return;
        }
    }
    else {
        // El canal existe: comprobar motivos por los que no se puede unir
        if (channel->getUserLimit() != -1 && channel->getUserCount() >= channel->getUserLimit()) {
            sendResponse(ctx, "471", channelName + " :Cannot join channel (+l)");
            return;
        }
        if (channel->getIsInviteOnly() && !channel->isUserInvited(ctx.sender)) {
            sendResponse(ctx, "473", channelName + " :Cannot join channel (+i)");
            return;
        }
        if (channel->isUserBanned(ctx.sender)) {
            sendResponse(ctx, "474", channelName + " :Cannot join channel (+b)");
            return;
        }
        if (channel->getKeyRequired() && channel->getKey() != key) {
            sendResponse(ctx, "475", channelName + " :Cannot join channel (+k)");
            return;
        }

        try {
            ctx.sender->joinChannel(channel, key);
        } catch (const IrcException &ie) {
            int code = ie.getCode();
            if (code == IRC_ERR_USER_ALREADY_IN_CHANNEL) {
                // Silencioso o se podría enviar otro numeric; dejamos sin respuesta
                return;
            } else if (code == IRC_ERR_CHANNEL_FULL) {
                sendResponse(ctx, "471", channelName + " :Cannot join channel (+l)");
            } else if (code == IRC_ERR_CHANNEL_INVITE_ONLY) {
                sendResponse(ctx, "473", channelName + " :Cannot join channel (+i)");
            } else if (code == IRC_ERR_USER_BANNED) {
                sendResponse(ctx, "474", channelName + " :Cannot join channel (+b)");
            } else if (code == IRC_ERR_INCORRECT_CHANNEL_KEY) {
                sendResponse(ctx, "475", channelName + " :Cannot join channel (+k)");
            } else {
                sendResponse(ctx, "475", channelName + " :Cannot join channel");
            }
            return;
        } catch (const std::exception &e) {
            sendResponse(ctx, "475", channelName + " :Cannot join channel");
            return;
        }
    }
}

static void sendJoinMessage(RequestContext &ctx, Channel *channel)
{
    std::string serverName = ctx.services.getServerName();
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string joinMsg = ":" + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " JOIN " + channel->getName() + "\r\n";
    ctx.services.sendToChannel(channel, joinMsg, NULL);
}

static void sendNamesList(RequestContext &ctx, Channel *channel)
{
    std::string serverName = ctx.services.getServerName();
    const std::map<int, User*>& usersMap = channel->getUsers();
    std::string namesList;
    for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
        if (!namesList.empty()) namesList += " ";
        if (channel->isUserOperator(uit->second))
            namesList += "@" + uit->second->getNickname();
        else
            namesList += uit->second->getNickname();
    }
    std::string namesReply = std::string(":") + serverName + " 353 " + ctx.sender->getNickname() + " = " + channel->getName() + " :" + namesList + "\r\n";
    ctx.services.sendToUser(ctx.sender, namesReply);

    std::string endNames = std::string(":") + serverName + " 366 " + ctx.sender->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
    ctx.services.sendToUser(ctx.sender, endNames);
}

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

    if (channelNames.empty()) {
        sendResponse(ctx, "461", "JOIN :Not enough parameters");
        return;
    }

    parseKeys(keysPart, keys);

    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        const std::string &channelName = channelNames[i];
        const std::string key = (i < keys.size() ? keys[i] : "");
        joinSingleChannel(ctx, channelName, key);
        Channel *channel = ctx.services.channels().getChannel(channelName);
        if (channel && channel->hasUser(ctx.sender)) {
            sendJoinMessage(ctx, channel);
            sendNamesList(ctx, channel);
        }
    }
}
