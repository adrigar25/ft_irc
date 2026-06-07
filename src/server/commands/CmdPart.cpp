/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:53 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:37:54 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPart.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>

static void parseChannelList(const std::string &channelsPart, std::vector<std::string> &out, RequestContext &ctx)
{
    std::istringstream ciss(channelsPart);
    std::string value;
    while (std::getline(ciss, value, ','))
    {
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
            value.erase(value.size() - 1, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            out.push_back(value);
        else if (!value.empty())
            ctx.services.sendToUser(ctx.sender, std::string("Invalid channel name: ") + value);
    }
}

static void sendResponse(RequestContext &ctx, const std::string &code, const std::string &message)
{
    std::string serverName = ctx.services.getServerName();
    std::string response = std::string(":") + serverName + " " + code + " " + ctx.sender->getNickname() + " " + message;
    ctx.services.sendToUser(ctx.sender, response);
}

static void partFromChannel(RequestContext &ctx, const std::string &channelName, const std::string &msg)
{
    Channel *channel = ctx.services.channels().getChannel(channelName);
    if (!channel) {
        sendResponse(ctx, "403", channelName + " :No such channel");
        return;
    }
    if (!channel->hasUser(ctx.sender)) {
        sendResponse(ctx, "442", channelName + " :You're not on that channel");
        return;
    }

    std::string serverName = ctx.services.getServerName();
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string out = std::string(":") + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " PART " + channel->getName();
    std::string text = msg;
    if (!text.empty() && text[0] == ':') text.erase(0,1);
    if (!text.empty() && text[text.size() - 1] == '\r') text.erase(text.size() - 1, 1);
    if (!text.empty()) out += std::string(" :") + text;
    out += "\r\n";

    ctx.services.sendToChannel(channel, out, NULL);

    ctx.sender->leaveChannel(channel);
}

static void splitChannelsAndMsg(const std::string &raw, std::string &outChannels, std::string &outMsg)
{
    outChannels = raw;
    outMsg.clear();
    size_t sp = raw.find(' ');
    if (sp != std::string::npos) {
        outChannels = raw.substr(0, sp);
        outMsg = raw.substr(sp + 1);
    }
}

void CmdPart::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::vector<std::string> channelNames;

    std::string channelsPart;
    std::string msgPart;
    splitChannelsAndMsg(ctx.rawLine, channelsPart, msgPart);

    parseChannelList(channelsPart, channelNames, ctx);

    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        partFromChannel(ctx, *it, msgPart);
    }
}
