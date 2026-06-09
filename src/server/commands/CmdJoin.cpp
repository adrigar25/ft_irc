/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:39 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 19:24:28 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdJoin.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
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
        if (params.empty())
            return false;
        size_t len = params.size();
        size_t start = 0;
        while (start <= len) {
            size_t pos = params.find(',', start);
            if (pos == std::string::npos)
                pos = len;
            std::string value = params.substr(start, pos - start);
            // trim leading spaces
            while (!value.empty() && value[0] == ' ')
                value.erase(0, 1);
            // trim trailing CR and spaces
            while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
                value.erase(value.size() - 1, 1);
            if (!value.empty())
                channelNames.push_back(value);
            if (pos == len)
                break;
            start = pos + 1;
        }
        return true;
}

static void parseKeys(const std::string &params, std::vector<std::string> &keys)
{
    if (params.empty())
        return;
    size_t len = params.size();
    size_t start = 0;
    while (start <= len) {
        size_t pos = params.find(',', start);
        if (pos == std::string::npos)
            pos = len;
        std::string value = params.substr(start, pos - start);
        // trim leading spaces
        while (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        // trim trailing CR and spaces
        while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
            value.erase(value.size() - 1, 1);
        // push the parsed value even if empty to preserve position for channels without keys
        keys.push_back(value);
        if (pos == len)
            break;
        start = pos + 1;
    }
    return;
}

static void joinSingleChannel(RequestContext &ctx, const std::string &channelName, const std::string &key)
{
    if (!ctx.sender) return;

    Channel *channel = ctx.services.channels().getChannel(channelName);

    if (!channel) {
        try {
            ctx.services.channels().createChannel(channelName, ctx.sender);
        } catch (const IrcException &ie) {
            int code = ie.getCode();
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
            else
                ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel");
            return;
        } catch (const std::exception &e) {
            ctx.services.sendResponse(ctx, "475", channelName + " :Cannot create/join channel");
            return;
        }
        channel = ctx.services.channels().getChannel(channelName);
        if (!channel) {
            ctx.services.sendResponse(ctx, "475", channelName + " :Failed to obtain channel after creation");
            return;
        }
    }
    else {
        try {
            ctx.sender->joinChannel(channel, key);
        } catch (const IrcException &ie) {
            int code = ie.getCode();
            if (code == IRC_ERR_USER_ALREADY_IN_CHANNEL) {
                ctx.services.sendResponse(ctx, "475", channelName + " :User already in channel");
                return;
            } else if (code == IRC_ERR_CHANNEL_FULL) {
                ctx.services.sendResponse(ctx, "471", channelName + " :Cannot join channel (+l)");
            } else if (code == IRC_ERR_CHANNEL_INVITE_ONLY) {
                ctx.services.sendResponse(ctx, "473", channelName + " :Cannot join channel (+i)");
            } else if (code == IRC_ERR_USER_BANNED) {
                ctx.services.sendResponse(ctx, "474", channelName + " :Cannot join channel (+b)");
            } else if (code == IRC_ERR_INCORRECT_CHANNEL_KEY) {
                ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel (+k)");
            } else {
                ctx.services.sendResponse(ctx, "475", channelName + " :Cannot join channel");
            }
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

    parseKeys(keysPart, keys);
    for(size_t i = 0; i < keys.size(); ++i) {
        std::cout << "Parsed key " << i << ": '" << keys[i] << "'" << std::endl;
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
