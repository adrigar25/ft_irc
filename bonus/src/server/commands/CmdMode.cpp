/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:10 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/24 19:11:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdMode.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "LineUtils.hpp"
#include "replies/Replies.hpp"

#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>

static bool isNumber(const std::string &s)
{
	for (size_t i = 0; i < s.size(); ++i)
		if (!std::isdigit(s[i]))
			return false;
	return !s.empty();
}

static void sendMode(RequestContext &ctx, const std::string &channelName, bool add, char mode, const std::string &param)
{
	std::string modes(1, add ? '+' : '-');
	Channel *ch = ctx.services.channels().getChannel(channelName);
	modes += mode;
	if (!param.empty())
		modes += " " + param;
	ctx.services.sendToChannel(ch, ctx.services.getServerPrefix() + " " + RPL_CHANNELMODEIS(ctx.sender->getNickname(), channelName, modes), nullptr);
}

/* ===================== HANDLERS ===================== */

static void modeI(Channel *ch, bool add)
{
	ch->setIsInviteOnly(add);
}

static void modeT(Channel *ch, bool add)
{
	ch->setTopicProtected(add);
}

static void modeM(Channel *ch, bool add)
{
	ch->setModerated(add);
}

static void modeK(Channel *ch, bool add, const std::string &key)
{
	if (add)
	{
		ch->setKey(key);
		ch->setKeyRequired(true);
	}
	else
	{
		ch->setKey("");
		ch->setKeyRequired(false);
	}
}

static void modeL(Channel *ch, bool add, int limit)
{
	if (add)
		ch->setUserLimit(limit);
	else
		ch->setUserLimit(-1);
}

static void appleUserMode(Channel *ch, bool add, User *u, char mode)
{
	if (mode == 'o')
	{
		if (add)
			ch->changeRole(u, "operator");
		else
			ch->removeRole(u, "operator");
	}
	else if (mode == 'v')
	{
		if (add)
			ch->changeRole(u, "voice");
		else
			ch->removeRole(u, "voice");
	}
	else if (mode == 'b')
	{
		if (add)
			ch->banUser(u);
		else
			ch->unbanUser(u);
	}
}

static void sendChannelModes(RequestContext &ctx, Channel *ch)
{
	std::string modes = "+";
	std::vector<std::string> params;
	std::string paramStr;
	ch->getChannelModes(modes, params);
	if (modes[1] == '\0')
		modes.clear();
	for (size_t i = 0; i < params.size(); ++i)
	{
		if (!paramStr.empty())
			paramStr += " ";
		paramStr += params[i];
	}
	ctx.services.sendResponse(ctx, RPL_CHANNELMODEIS(ctx.sender->getNickname(), ch->getName(), modes + (paramStr.empty() ? "" : " " + paramStr)));
}

static void executeMode(char m, bool add, Channel *ch, User *u, const std::string &param)
{
	switch (m)
	{
	case 'i':
		modeI(ch, add);
		break;
	case 't':
		modeT(ch, add);
		break;
	case 'm':
		modeM(ch, add);
		break;
	case 'k':
		modeK(ch, add, param);
		break;
	case 'l':
		if (!param.empty() && !isNumber(param))
		{
			ctx.services.sendResponse(ctx, ERR_INVALIDMODEPARAM(ctx.sender->getNickname(), channelName));
			return;
		}
		modeL(ch, add, param.empty() ? -1 : std::atoi(param.c_str()));
		break;
	case 'o':
	case 'v':
	case 'b':
	{
		User *u = ctx.services.users().findByNick(param);
		if (!u)
		{
			ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), params[p - 1]));
			return;
		}
		appleUserMode(ch, add, u, m);
		break;
	}
	default:
		ctx.services.sendResponse(ctx, ERR_UNKNOWNMODE(ctx.sender->getNickname(), std::string(1, m)));
		break;
	}
}
/* ===================== EXEC ===================== */

void CmdMode::execute(RequestContext &ctx)
{
	if (!ctx.sender || ctx.rawLine.empty())
		return;

	std::vector<std::string> parts = split(ctx.rawLine, ' ');

	if (parts.size() < 1)
	{
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "MODE"));
		return;
	}

	std::string channelName = parts[0];
	std::string modes = parts[1];

	Channel *ch = ctx.services.channels().getChannel(channelName);
	if (!ch)
	{
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}

	if (modes.empty() || (modes[0] != '+' && modes[0] != '-'))
	{
		sendChannelModes(ctx, ch);
		return;
	}

	if (!ch->isUserOperator(ctx.sender))
	{
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
		return;
	}

	std::vector<std::string> params;
	for (size_t i = 2; i < parts.size(); ++i)
		params.push_back(parts[i]);

	size_t p = 0;
	bool add = true;
	std::string param;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		char m = modes[i];

		if (m == '+')
		{
			add = true;
			continue;
		}
		if (m == '-')
		{
			add = false;
			continue;
		}

		if (add && (m == 'k' || m == 'l' || m == 'o' || m == 'v' || m == 'b'))
		{
			if (p >= params.size())
			{
				ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "MODE"));
				return;
			}
			param = params[p++];
		}

		executeMode(m, add, ch, ctx.sender, param);
		sendMode(ctx, channelName, add, m, param);
		param.clear();
	}
}
