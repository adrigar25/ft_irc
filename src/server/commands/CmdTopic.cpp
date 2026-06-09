/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:07 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:21:27 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdTopic.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "LineUtils.hpp"

static void parseTopicParams(const std::string &params, std::string &outChannelName, std::string &outTopic)
{
    size_t sp = params.find(' ');
    outChannelName = (sp == std::string::npos) ? params : params.substr(0, sp);
    outTopic = (sp == std::string::npos) ? "" : params.substr(sp + 1);
    if (!outTopic.empty() && outTopic[0] == ':') outTopic.erase(0,1);
}

static void sendResponse(RequestContext &ctx, const std::string &code, const std::string &message)
{
    std::string serverName = ctx.services.getServerName();
    std::string response = std::string(":") + serverName + " " + code + " " + ctx.sender->getNickname() + " " + message;
    ctx.services.sendToUser(ctx.sender, response);
}

void CmdTopic::execute(RequestContext &ctx)
{

    std::string channelName;
    std::string topic;
    std::string params;

    if (!ctx.sender)
            return;

    params = trim(ctx.rawLine, " \r");

    std::string serverName = ctx.services.getServerName();
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";

    parseTopicParams(params, channelName, topic);

    if(channelName.empty()) {
        sendResponse(ctx, "461", "TOPIC :Not enough parameters");
        return;
    }

    Channel *targetChannel = ctx.services.channels().getChannel(channelName);

    if(!targetChannel) {
        sendResponse(ctx, "403", channelName + " :No such channel");
        return;
    }
    if(topic.empty()) {
        if(targetChannel->getTopic().empty())
            sendResponse(ctx, "331", channelName + " :No topic is set");
        else
            sendResponse(ctx, "332", channelName + " :" + targetChannel->getTopic());
        return;
    }
    if(!targetChannel->hasUser(ctx.sender)) {
        sendResponse(ctx, "442", channelName + " :You're not on that channel");
        return;
    }
    if(!targetChannel->isUserOperator(ctx.sender) && targetChannel->getTopicProtected()) {
        sendResponse(ctx, "482", channelName + " :You're not channel operator");
        return;
    }
    targetChannel->setTopic(topic);
    std::string response = ":" + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " TOPIC " + channelName + " :" + topic;
    ctx.services.sendToChannel(targetChannel, response);
}
