/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:39 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:20:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdJoin.hpp"
#include "RequestContext.hpp"
#include "LineUtils.hpp"
#include "Server.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstring>

static void handleJoinError(RequestContext &ctx, const std::string &channelName, int code)
{
    if (code == IRC_ERR_CHANNEL_FULL)
        ctx.services.sendResponse(ctx, "471", channelName + " :Cannot join channel (+l)");
    else if (code == IRC_ERR_CHANNEL_INVITE_ONLY)
        ctx.services.sendResponse(ctx, "473", channelName + " :Cannot join channel (+i)");
    else if (code == IRC_ERR_USER_BANNED)
        ctx.services.sendResponse(ctx, "474", channelName + " :Cannot join channel (+b)");
    else if (code == IRC_ERR_INCORRECT_CHANNEL_KEY)
        ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel (+k)");
    else if (code == IRC_ERR_BAD_CHANNEL_NAME)
        ctx.services.sendResponse(ctx, "476", channelName + " :Bad channel mask");
    else if (code == IRC_ERR_USER_ALREADY_IN_CHANNEL)
        ctx.services.sendResponse(ctx, "475", channelName + " :User already in channel");
    else
        ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel");
}


static void joinSingleChannel(RequestContext &ctx, const std::string &channelName, const std::string &key)
{
    if (!ctx.sender) return;

    Channel *channel = ctx.services.channels().getChannel(channelName);

    if (!channel) {
        try {
            ctx.services.channels().createChannel(channelName, ctx.sender);
        } catch (const IrcException &ie) {
            handleJoinError(ctx, channelName, ie.getCode());
            return;
        } catch (const std::exception &e) {
            ctx.services.sendResponse(ctx, "475", channelName + " :Cannot create channel");
            return;
        }
    }
    else {
        try {
            ctx.sender->joinChannel(channel, key);
        } catch (const IrcException &ie) {
            handleJoinError(ctx, channelName, ie.getCode());
            return;
        } catch (const std::exception &e) {
            ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel");
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

void CmdJoin::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = ctx.rawLine;
    std::string keysPart;
   
    channelNames = split(trim(channelsPart, " "), ',');
    keys = split(trim(keysPart, " "), ',');

    if (channelNames.empty()) {
        ctx.services.sendResponse(ctx, "461", "JOIN :Not enough parameters");
        return;
    }

    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        const std::string &channelName = channelNames[i];
        const std::string key = (i < keys.size() ? keys[i] : "");
        joinSingleChannel(ctx, channelName, key);
        Channel *channel = ctx.services.channels().getChannel(channelName);
        if (channel && channel->hasUser(ctx.sender)) {
            sendJoinMessage(ctx, channel);
            ctx.services.getServer()->sendNamesList(ctx.sender, channel);
        }
    }
}
