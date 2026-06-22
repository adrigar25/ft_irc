/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:43 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/22 18:31:38 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPart.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "LineUtils.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>
#include "replies/Replies.hpp"

static void parseChannelList(RequestContext &ctx, const std::string &channelsPart, std::vector<std::string> &out)
{
	out = split(channelsPart, ',');
	for (std::vector<std::string>::iterator it = out.begin(); it != out.end(); ++it)
	{
		trim(*it, " \t\r\n");
		if (!it->empty() && (*it)[0] != '#')
		{
			ctx.services.sendResponse(ctx, ERR_BADCHANMASK(ctx.sender->getNickname(), *it));
			it = out.erase(it);
			--it;
		}
	}
}

static void partFromChannel(RequestContext &ctx, const std::string &channelName, const std::string &msg)
{
	Channel *channel = ctx.services.channels().getChannel(channelName);
	if (!channel)
	{
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}
	if (!channel->hasUser(ctx.sender))
	{
		ctx.services.sendResponse(ctx, ERR_NOTONCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}

	std::string uname = ctx.sender->getUsername().empty() ? "~" : ctx.sender->getUsername();
	std::string out = ":" + ctx.services.getUserPrefix(ctx.sender) + " PART " + channelName + " " + msg + "\r\n";

	ctx.services.sendToChannel(channel, out, NULL);
	ctx.sender->leaveChannel(channel);

	if (channel->isEmpty())
		ctx.services.channels().deleteChannel(channelName);
}

static void splitChannelsAndMsg(const std::string &raw, std::string &outChannels, std::string &outMsg)
{
	std::vector<std::string> parts = split(raw, ' ');
	outChannels = parts[0];
	for (size_t i = 1; i < parts.size(); ++i)
	{
		outMsg += parts[i];
		if (i < parts.size() - 1)
			outMsg += " ";
	}
}

void CmdPart::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;
	std::vector<std::string> channelNames;

	std::string channelsPart = "";
	std::string msgPart = "";
	splitChannelsAndMsg(ctx.rawLine, channelsPart, msgPart);

	if (channelsPart.empty() || (msgPart.size() > 0 && msgPart[0] != ':'))
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "PART"));
		return;
	}

	parseChannelList(ctx, channelsPart, channelNames);

	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
		partFromChannel(ctx, *it, msgPart);
}
