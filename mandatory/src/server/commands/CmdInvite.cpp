/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:21 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 18:06:30 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdInvite.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "LineUtils.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "replies/Replies.hpp"
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

static bool canInvite(RequestContext &ctx, User* targetUser, const Channel* channel)
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

#include <iostream>

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
