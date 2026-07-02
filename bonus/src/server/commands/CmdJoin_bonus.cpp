/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:17 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:25:38 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdJoin_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "LineUtils_bonus.hpp"
#include "Server_bonus.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include "replies/Replies_bonus.hpp"

/**
 * @brief Handles join errors
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param code The error code
 */
static void handleJoinError(RequestContext &ctx, const std::string &channelName, int code)
{
	if (code == IRC_ERR_CHANNEL_FULL)
		ctx.services.sendResponse(ctx, ERR_CHANNELISFULL(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_CHANNEL_INVITE_ONLY)
		ctx.services.sendResponse(ctx, ERR_INVITEONLYCHAN(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_USER_BANNED)
		ctx.services.sendResponse(ctx, ERR_BANNEDFROMCHAN(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_INCORRECT_CHANNEL_KEY)
		ctx.services.sendResponse(ctx, ERR_BADCHANNELKEY(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_BAD_CHANNEL_NAME)
		ctx.services.sendResponse(ctx, ERR_BADCHANMASK(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_USER_ALREADY_IN_CHANNEL)
		ctx.services.sendResponse(ctx, ERR_USERONCHANNEL(ctx.sender->getNickname(), ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_BANNEDFROMCHAN)
		ctx.services.sendResponse(ctx, ERR_BANNEDFROMCHAN(ctx.sender->getNickname(), channelName));
	else if (code == IRC_ERR_ISINVITEONLYCHAN)
		ctx.services.sendResponse(ctx, ERR_INVITEONLYCHAN(ctx.sender->getNickname(), channelName));
}

/**
 * @brief Joins a single channel
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param key The key for the channel
 * @return true if the user joined the channel, false otherwise
 */
static bool joinSingleChannel(RequestContext &ctx, const std::string &channelName, const std::string &key)
{
	if (!ctx.sender)
		return false;

	Channel *channel = ctx.services.channels().getChannel(channelName);

	try
	{
		if (!channel)
			ctx.services.channels().createChannel(channelName, ctx.sender);
		else
			ctx.sender->joinChannel(channel, key);
	}
	catch (const IrcException &ie)
	{
		handleJoinError(ctx, channelName, ie.getCode());
		return false;
	}
	return true;
}

/**
 * @brief Sends a join message to the channel
 * @param ctx The request context
 * @param channel The channel to send the message to
 */
static void sendJoinMessage(RequestContext &ctx, Channel *channel)
{
	std::string joinMsg = RPL_JOIN(ctx.services.getUserPrefix(ctx.sender), channel->getName());
	ctx.services.sendToChannel(channel, joinMsg, NULL);
}

/**
 * @brief Executes the JOIN command
 *  - Handles the JOIN command for joining channels.
 *  - Supports joining multiple channels with optional keys.
 *  - Sends appropriate error messages for join failures.
 *  - Sends join messages to the channel and lists names of users in the channel.
 * @param ctx The request context
 */
void CmdJoin::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::vector<std::string> channelNames;
	std::vector<std::string> keys;

	std::vector<std::string> parts = split(ctx.rawLine, ' ');
	if (parts.size() < 1) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "JOIN"));
		return;
	}
	channelNames = split(trim(parts[0], " "), ',');
	if(parts.size() > 1)
		keys = split(trim(parts[1], " "), ',');

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		const std::string &channelName = channelNames[i];
		const std::string key = (i < keys.size() ? keys[i] : "");
		bool joined = joinSingleChannel(ctx, channelName, key);
		Channel *channel = ctx.services.channels().getChannel(channelName);
		if (joined && channel && channel->hasUser(ctx.sender)) {
			sendJoinMessage(ctx, channel);
			ctx.services.sendNamesList(ctx, ctx.sender, channel);
		}
	}
}
