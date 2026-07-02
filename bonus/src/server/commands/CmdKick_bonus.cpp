/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:19:52 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/28 18:26:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdKick_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "LineUtils_bonus.hpp"
#include "Services_bonus.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include "replies/Replies_bonus.hpp"

/**
 * @brief Validates the KICK command
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param targetNick The nickname of the user to kick
 * @param outChannel The channel to kick from
 * @param outTarget The user to kick
 * @return true if the command is valid, false otherwise
 */
static bool validateKick(
	RequestContext &ctx,
	const std::string &channelName,
	const std::string &targetNick,
	Channel *&outChannel,
	User *&outTarget)
{
	outChannel = ctx.services.channels().getChannel(channelName);
	if (!outChannel)
	{
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return false;
	}

	outTarget = ctx.services.users().findByNick(targetNick);
	if (!outTarget)
	{
		ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), targetNick));
		return false;
	}

	if (!outChannel->isUserOperator(ctx.sender))
	{
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
		return false;
	}

	if (!outChannel->hasUser(outTarget))
	{
		ctx.services.sendResponse(ctx, ERR_USERNOTINCHANNEL(ctx.sender->getNickname(), targetNick, channelName));
		return false;
	}

	return true;
}

/**
 * @brief Kicks a single user from a channel
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param nick The nickname of the user to kick
 * @param reason The reason for the kick
 */
static void kickSingle(
	RequestContext &ctx,
	const std::string &channelName,
	const std::string &nick,
	const std::string &reason)
{
	Channel *channel = NULL;
	User *target = NULL;

	if (!validateKick(ctx, channelName, nick, channel, target))
		return;

	std::string msg = RPL_KICK(ctx.services.getUserPrefix(ctx.sender), channel->getName(), target->getNickname(), reason.empty() ? "No reason" : reason);
	ctx.services.sendToChannel(channel, msg, NULL);
	ctx.services.channels().removeUserFromChannel(channel->getName(), target);
}

/**
 * @brief Handles kicking multiple users from a single channel
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param users The list of users to kick
 * @param reason The reason for the kick
 */
static void handleKick1N(
	RequestContext &ctx,
	const std::string &channelName,
	const std::vector<std::string> &users,
	const std::string &reason)
{
	for (size_t i = 0; i < users.size(); ++i)
	{
		std::string nick = trim(users[i], " \r");

		if (!nick.empty())
			kickSingle(ctx, channelName, nick, reason);
	}
}

/**
 * @brief Handles kicking a single user from multiple channels
 * @param ctx The request context
 * @param channels The list of channels to kick from
 * @param nick The nickname of the user to kick
 * @param reason The reason for the kick
 */
static void handleKickN1(
	RequestContext &ctx,
	const std::vector<std::string> &channels,
	const std::string &nick,
	const std::string &reason)
{
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string channel = trim(channels[i], " \r");

		if (!channel.empty())
			kickSingle(ctx, channel, nick, reason);
	}
}

/**
 * @brief Executes the KICK command
 *  - Handles the KICK command for kicking users from channels.
 *  - Supports kicking multiple users from a single channel or a single user from multiple channels.
 *  - Sends appropriate error messages for kick failures.
 * @param ctx The request context
 */
void CmdKick::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;

	std::vector<std::string> parts = split(trim(ctx.rawLine, " \r"), ' ');
	if (parts.size() < 2)
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));
		return;
	}

	std::vector<std::string> channels = split(trim(parts[0], " "), ',');
	std::vector<std::string> users = split(trim(parts[1], " "), ',');

	std::string reason = "";
	size_t pos = ctx.rawLine.find(" :");
	if (pos != std::string::npos)
		reason = ctx.rawLine.substr(pos + 2);

	if (channels.empty() || users.empty())
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));
		return;
	}

	if (channels.size() == 1)
		handleKick1N(ctx, channels[0], users, reason);
	else if (users.size() == 1)
		handleKickN1(ctx, channels, users[0], reason);
	else
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));
}
