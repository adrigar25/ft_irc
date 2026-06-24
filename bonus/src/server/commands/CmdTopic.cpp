/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:49 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/22 18:32:04 by agarcia          ###   ########.fr       */
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
#include "replies/Replies.hpp"

static void parseTopicParams(const std::string &params, std::string &outChannelName, std::string &outTopic)
{
	size_t sp = params.find(' ');

	outChannelName = (sp == std::string::npos) ? params : params.substr(0, sp);

	outTopic = (sp == std::string::npos) ? "" : params.substr(sp + 1);
}

void CmdTopic::execute(RequestContext &ctx)
{

	std::string channelName;
	std::string topic;
	std::string params;
	std::string response;

	if (!ctx.sender)
		return;

	params = trim(ctx.rawLine, " \r");

	std::string serverName = ctx.services.getServerName();
	std::string uname = ctx.sender->getUsername();
	if (uname.empty())
		uname = "~";

	parseTopicParams(params, channelName, topic);

	if (channelName.empty())
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "TOPIC"));
		return;
	}

	Channel *targetChannel = ctx.services.channels().getChannel(channelName);

	if (!targetChannel)
	{
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}

	if (topic.empty())
	{
		if (targetChannel->getTopic().empty())
			ctx.services.sendResponse(ctx, RPL_NOTOPIC(ctx.sender->getNickname(), channelName));
		else
			ctx.services.sendResponse(ctx, RPL_TOPIC(ctx.sender->getNickname(), channelName, targetChannel->getTopic()));
		return;
	}

	if (topic.length() == 1 && topic[0] == ':')
		topic = "";
	else if (topic.length() > 1 && topic[0] == ':')
		topic = topic.substr(1);

	if (!targetChannel->isUserOperator(ctx.sender) && targetChannel->getTopicProtected())
	{
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
		return;
	}
	targetChannel->setTopic(topic);
	response = ":" + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " TOPIC " + channelName + " :" + topic;
	ctx.services.sendToChannel(targetChannel, response);
}
