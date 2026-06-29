/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:43 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:35:14 by agarcia          ###   ########.fr       */
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

/**
 * @brief Parses a list of channels from a raw line.
 * @param ctx The request context.
 * @param channelsPart The part of the line containing channel names.
 * @param out The vector to store the parsed channel names.
 */
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

/**
 * @brief Removes a user from a channel.
 * @param ctx The request context.
 * @param channelName The name of the channel.
 * @param msg The parting message.
 */
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

/**
 * @brief Splits the raw line into channels and message parts.
 * @param raw The raw line.
 * @param outChannels The string to store the channel names.
 * @param outMsg The string to store the parting message.
 */
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

/**
 * @brief Executes the PART command.
 * - Handles the PART command for leaving channels.
 * - Validates the channel names and sends appropriate responses for errors.
 * - Removes the user from the specified channels and sends parting messages.
 * @param ctx The request context.
 */
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
