/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:40 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/19 10:50:05 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdMsg_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "Channel_bonus.hpp"
#include "User_bonus.hpp"
#include "LineUtils_bonus.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include "replies/Replies_bonus.hpp"

CmdMsg::CmdMsg(bool isNotice) : _isNotice(isNotice) {}

static bool parseMsgParams(const std::vector<std::string> &parts, std::vector<std::string> &outTargets, std::string &outMsg)
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

static void dispatchMsg(RequestContext &ctx, const std::string &target, const std::string &msg, bool isNotice)
{

	std::string out = ":" + ctx.services.getUserPrefix(ctx.sender) + " " + (isNotice ? RPL_NOTICE(target, msg) : RPL_PRIVMSG(target, msg));
	if (target[0] == '#')
	{
		Channel *channel = ctx.services.channels().getChannel(target);
		if (!channel)
		{
			if (!isNotice)
				ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
			return;
		}
		if (!channel->hasUser(ctx.sender))
		{
			if (!isNotice)
				ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
			return;
		}
		if (channel->getIsModerated() && !channel->isUserVoice(ctx.sender) && !channel->isUserOperator(ctx.sender))
		{
			if (!isNotice)
				ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
			return;
		}
		ctx.services.sendToChannel(channel, out, ctx.sender);
	}
	else
	{
		User *dest = ctx.services.users().findByNick(target);
		if (!dest)
		{
			if (!isNotice)
				ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
			return;
		}
		ctx.services.sendToUser(dest, out);
	}
}

void CmdMsg::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;
	const std::vector<std::string> parts = split(ctx.rawLine, ' ');
	std::vector<std::string> targets;
	std::string msg = "";

	parseMsgParams(parts, targets, msg);

	if (targets.empty())
	{
		if (!_isNotice)
			ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}
	else if (msg.empty())
	{
		if (!_isNotice)
			ctx.services.sendResponse(ctx, ERR_NOTEXTTOSEND(ctx.sender->getNickname(), "PRIVMSG"));
		return;
	}

	for (size_t i = 0; i < targets.size(); ++i)
	{
		const std::string &target = targets[i];
		dispatchMsg(ctx, target, msg, _isNotice);
	}
}
