/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:57 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:02:58 by agarcia          ###   ########.fr       */
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

static void sendMode(RequestContext &ctx, const std::string &channelName,
					 char sign, char mode, const std::string &param)
{
	std::string modes(1, sign);
	modes += mode;
	if (!param.empty())
		modes += " " + param;
	ctx.services.sendResponse(ctx, RPL_CHANNELMODEIS(ctx.sender->getNickname(), channelName, modes));
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

static void modeO(Channel *ch, bool add, User *u)
{
	if (add)
		ch->changeRole(u, "operator");
	else
		ch->removeRole(u, "operator");
}

static void modeV(Channel *ch, bool add, User *u)
{
	if (add)
		ch->changeRole(u, "voice");
	else
		ch->removeRole(u, "voice");
}

static void modeB(Channel *ch, bool add, User *u)
{
	if (add)
		ch->banUser(u);
	else
		ch->unbanUser(u);
}

static void sendChannelModes(RequestContext &ctx, Channel *ch)
{
	std::string modes = "+";
	std::vector<std::string> params;
	std::string paramStr;
	ch->getChannelModes(modes, params);
	if(modes[1] == '\0')
		modes.clear();
	for (size_t i = 0; i < params.size(); ++i)
	{
		if (!paramStr.empty())
			paramStr += " ";
		paramStr += params[i];
	}
	ctx.services.sendResponse(ctx, RPL_CHANNELMODEIS(ctx.sender->getNickname(), ch->getName(), modes + (paramStr.empty() ? "" : " " + paramStr)));
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

	if (!ch->isUserOperator(ctx.sender))
	{
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
		return;
	}

	if(modes.empty() || (modes[0] != '+' && modes[0] != '-'))
	{
		sendChannelModes(ctx, ch);
		return;
	}

	std::vector<std::string> params;
	for (size_t i = 2; i < parts.size(); ++i)
		params.push_back(parts[i]);

	size_t p = 0;
	bool add = true;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		char m = modes[i];

		if (m == '+') { add = true; continue; }
		if (m == '-') { add = false; continue; }

		std::string param;

		switch (m)
		{
			case 'i':
				modeI(ch, add);
				sendMode(ctx, channelName, add ? '+' : '-', 'i', "");
				break;

			case 't':
				modeT(ch, add);
				sendMode(ctx, channelName, add ? '+' : '-', 't', "");
				break;

			case 'm':
				modeM(ch, add);
				sendMode(ctx, channelName, add ? '+' : '-', 'm', "");
				break;

			case 'k':
				if (add)
				{
					if (p >= params.size())
					{
						ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "MODE"));
						return;
					}
					param = params[p++];
				}
				modeK(ch, add, param);
				sendMode(ctx, channelName, add ? '+' : '-', 'k', param);
				break;

			case 'l':
				if (add)
				{
					if (p >= params.size())
					{
						ctx.services.sendToUser(ctx.sender, ERR_INVALIDMODEPARAM(ctx.sender->getNickname(), channelName));
						return;
					}
					if (!isNumber(params[p]))
					{
						ctx.services.sendResponse(ctx, ERR_INVALIDMODEPARAM(ctx.sender->getNickname(), channelName));
						return;
					}
					int limit = std::atoi(params[p++].c_str());
					modeL(ch, add, limit);
					sendMode(ctx, channelName, '+', 'l', params[p - 1]);
				}
				else
				{
					modeL(ch, add, -1);
					sendMode(ctx, channelName, '-', 'l', "");
				}
				break;

			case 'o':
			case 'v':
			case 'b':
			{
				if (p >= params.size())
				{
					ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "MODE"));
					return;
				}

				User *u = ctx.services.users().findByNick(params[p++]);
				if (!u)
				{
					ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), params[p - 1]));
					continue;
				}

				if (m == 'o') modeO(ch, add, u);
				if (m == 'v') modeV(ch, add, u);
				if (m == 'b') modeB(ch, add, u);

				std::string nick = u->getNickname();
				sendMode(ctx, channelName, add ? '+' : '-', m, nick);
				break;
			}

			default:
				ctx.services.sendResponse(ctx, ERR_UNKNOWNMODE(ctx.sender->getNickname(), std::string(1, m)));
				break;
		}
	}
}
