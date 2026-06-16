/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:40 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 19:57:33 by agarcia          ###   ########.fr       */
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

	bool found = false;
	for (size_t i = 1; i < parts.size(); ++i)
	{
		if (!found && parts[i][0] == ':')
		{
			found = true;
			outMsg = parts[i].substr(1);
		}
		else if (found)
		{
			outMsg += " " + parts[i];
		}
	}

	return true;
}

static void dispatchPrivmsg(RequestContext &ctx, const std::string &target, const std::string &msg)
{

	if(target.empty()) {
		ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}

	std::cout << "Dispatching PRIVMSG to target: " << target << " with message: " << msg << std::endl;
	std::string out = ":" + ctx.services.getUserPrefix(ctx.sender) + " " + RPL_PRIVMSG(target, msg);
	std::cout << "OUT: " << out << std::endl;

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
	std::string msg = "";


	if(parts.empty()) {
		ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}else if(parts.size() == 1 || (parts.size() > 1 && parts[1][0] != ':')) {
		ctx.services.sendResponse(ctx, ERR_NOTEXTTOSEND(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}

	parsePrivmsgParams(parts, targets, msg);
	std::cout << "Executing PRIVMSG command from user: " << ctx.sender->getNickname() << " with raw line: " << ctx.rawLine << " msg: " << msg << std::endl;

	for (size_t i = 0; i < targets.size(); ++i) {
		const std::string &target = targets[i];
		dispatchPrivmsg(ctx, target, msg);
	}
}
