/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:20:27 by adriescr         ###   ########.fr       */
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
#include "replies/Replies.hpp"

static void parseChannelList(const std::string &channelsPart, std::vector<std::string> &out, RequestContext &ctx)
{
	std::istringstream ciss(channelsPart);
	std::string value;
	while (std::getline(ciss, value, ','))
	{
		if (!value.empty() && value[0] == ' ') value.erase(0, 1);
		while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == ' '))
			value.erase(value.size() - 1, 1);
		if (!value.empty() && value[0] == '#')
			out.push_back(value);
		else if (!value.empty())
			ctx.services.sendToUser(ctx.sender, std::string("Invalid channel name: ") + value);
	}
}


static void partFromChannel(RequestContext &ctx, const std::string &channelName, const std::string &msg)
{
	Channel *channel = ctx.services.channels().getChannel(channelName);
	if (!channel) {
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}
	if (!channel->hasUser(ctx.sender)) {
		ctx.services.sendResponse(ctx, ERR_NOTONCHANNEL(ctx.sender->getNickname(), channelName));
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
	std::istringstream iss(raw);
	std::string channelsPart;
	if (!std::getline(iss, channelsPart, ' ')) {
		outChannels = raw;
		outMsg = "";
		return;
	}else {
		outChannels = channelsPart;
		std::string rest;
		if (std::getline(iss, rest))
			outMsg = rest;
		else
			outMsg = "";
	}
}

void CmdPart::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::vector<std::string> channelNames;

	std::string channelsPart;
	std::string msgPart;
	splitChannelsAndMsg(ctx.rawLine, channelsPart, msgPart);

	if(channelsPart.empty())
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "PART"));
		return ;
	}

	if(msgPart.size() > 0 && msgPart[0] != ':')
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "PART"));
		return ;
	}

	parseChannelList(channelsPart, channelNames, ctx);

	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
	{
		partFromChannel(ctx, *it, msgPart);
	}
}
