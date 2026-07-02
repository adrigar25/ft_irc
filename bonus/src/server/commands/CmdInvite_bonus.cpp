/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:21 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:24:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdInvite_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "LineUtils_bonus.hpp"
#include "User_bonus.hpp"
#include "Channel_bonus.hpp"
#include "replies/Replies_bonus.hpp"
#include <string>
#include <cstring>

static void parseInviteParams(const std::string &params, std::string &outTargetNick, std::string &outChannelName)
{
	std::vector<std::string> parts = split(trim(params, " \r"), ' ');
	if (parts.size() >= 2) {
		outTargetNick = parts[0];
		outChannelName = parts[1];
	}
}

/**
 * @brief Checks if a user can invite another user to a channel
 * @param ctx The request context
 * @param targetUser The user to invite
 * @param channel The channel to invite to
 * @return true if the user can invite, false otherwise
 */
static bool canInvite(RequestContext &ctx, const User* targetUser, const Channel* channel)
{
	
	if (!channel) {
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), "channel"));
		return false;
	}

	if (!targetUser) {
		ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), "target"));
		return false;
	}
	
	if(!channel->hasUser(ctx.sender)) {
		ctx.services.sendResponse(ctx, ERR_NOTONCHANNEL(ctx.sender->getNickname(), channel->getName()));
		return false;
	}
	
	if (!channel->isUserOperator(ctx.sender)) {
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channel->getName()));
		return false;
	}
	
	if (channel->hasUser(targetUser)) {
		ctx.services.sendResponse(ctx, ERR_USERONCHANNEL(ctx.sender->getNickname(), targetUser->getNickname(), channel->getName()));
		return false;
	}
	return true;
}

/**
 * @brief Executes the INVITE command
 *  - Checks if the sender can invite the target user to the specified channel.
 *  - If valid, sends an invitation message to the target user and a confirmation to the sender.
 *  - If invalid, sends the appropriate error response to the sender.
 * @param ctx The request context
 */
void CmdInvite::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string targetNick;
	std::string channelName;
	parseInviteParams(ctx.rawLine, targetNick, channelName);
	if (targetNick.empty() || channelName.empty()) 
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "INVITE"));
		return;
	}

	User* targetUser = ctx.services.users().findByNick(targetNick);
	Channel* channel = ctx.services.channels().getChannel(channelName);

	if (!canInvite(ctx, targetUser, channel))
		return;
		
	channel->inviteUser(targetUser);
	ctx.services.sendToUser(targetUser, std::string(":") + ctx.services.getUserPrefix(ctx.sender) + " INVITE " + targetUser->getNickname() + " :" + channel->getName() + "\r\n");
	ctx.services.sendResponse(ctx, RPL_INVITING(ctx.sender->getNickname(), targetUser->getNickname(), channel->getName()));
}
