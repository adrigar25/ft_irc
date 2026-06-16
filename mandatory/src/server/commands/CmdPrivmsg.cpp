/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:20 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:40:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPrivmsg.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "LineUtils.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include "replies/Replies.hpp"

static bool parsePrivmsgParams(const std::vector<std::string> &parts, std::vector<std::string> &outTargets, std::string &outMsg)
{
	outTargets = split(parts[0], ',');
	
	if(parts.size() > 1) {
		if(parts[1][0] == ':') {
			outMsg = parts[1].substr(1);
		} else {
			outMsg = parts[1];
		}
	} else {
		outMsg = "";
	}
	
	return true;
}

static void dispatchPrivmsg(RequestContext &ctx, const std::string &target, const std::string &msg)
{

	if(target.empty()) {
		ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}

	std::string out = ":" + ctx.services.getUserPrefix(ctx.sender) + " " + RPL_PRIVMSG(target, msg);

	if(target[0] == '#') {
		Channel *channel = ctx.services.channels().getChannel(target);
		if (!channel) {
			ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
			return;
		}
		if(!channel->hasUser(ctx.sender)) {
			ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
			return;
		}
		if(channel->getIsModerated() && !channel->isUserVoice(ctx.sender) && !channel->isUserOperator(ctx.sender)) {
			ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
			return;
		}
		ctx.services.sendToChannel(channel, out, ctx.sender);
	} else {
		User *dest = ctx.services.users().findByNick(target);
		if (!dest) {
			ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
			return;
		}
		ctx.services.sendToUser(dest, out);
	}
}

void CmdPrivmsg::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	const std::vector<std::string> parts = split(ctx.rawLine, ' ');
	std::vector<std::string> targets;
	std::string msg;
	
	if(parts.empty()) {
		ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}else if(parts.size() == 1 || (parts.size() > 1 && parts[1][0] != ':')) {
		ctx.services.sendResponse(ctx, ERR_NOTEXTTOSEND(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}
	
	parsePrivmsgParams(parts, targets, msg);
	
	for (size_t i = 0; i < targets.size(); ++i) {
		const std::string &target = targets[i];
		dispatchPrivmsg(ctx, target, msg);
	}
}
