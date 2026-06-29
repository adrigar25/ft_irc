/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:10 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/28 18:27:51 by agarcia          ###   ########.fr       */
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
#include <fnmatch.h>

static bool isNumber(const std::string &s)
{
	for (size_t i = 0; i < s.size(); ++i)
		if (!std::isdigit(s[i]))
			return false;
	return !s.empty();
}

/**
 * @brief Sends the mode information for a channel to the user
 * @param ctx The request context
 * @param channelName The name of the channel
 * @param add Whether to add or remove the mode
 * @param mode The mode character
 * @param param The parameter for the mode
 */
static void sendMode(RequestContext &ctx, const std::string &channelName, bool add, char mode, const std::string &param)
{
	std::string modes(1, add ? '+' : '-');
	Channel *ch = ctx.services.channels().getChannel(channelName);
	modes += mode;
	if (!param.empty())
		modes += " " + param;
	ctx.services.sendToChannel(ch, ctx.services.getServerPrefix() + " " + RPL_CHANNELMODEIS(ctx.sender->getNickname(), channelName, modes), nullptr);
}


/**
 * @brief Sets the invite-only mode for a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 */
static void modeI(Channel *ch, bool add)
{
	ch->setIsInviteOnly(add);
}

/**
 * @brief Sets the topic protection mode for a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 */
static void modeT(Channel *ch, bool add)
{
	ch->setTopicProtected(add);
}

/**
 * @brief Sets the moderated mode for a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 */
static void modeM(Channel *ch, bool add)
{
	ch->setModerated(add);
}

/**
 * @brief Sets the key mode for a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 * @param key The key for the channel
 */
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

/**
 * @brief Sets the user limit mode for a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 * @param limit The user limit for the channel
 */
static void modeL(Channel *ch, bool add, int limit)
{
	if (add)
		ch->setUserLimit(limit);
	else
		ch->setUserLimit(-1);
}

/**
 * @brief Applies a user mode (operator or voice) to a user in a channel
 * @param ch The channel
 * @param add Whether to add or remove the mode
 * @param u The user
 * @param mode The mode character ('o' for operator, 'v' for voice)
 */
static void applyUserMode(Channel *ch, bool add, User *u, char mode)
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
}

/**
 * @brief Checks if a string contains wildcard characters
 * @param mask The string to check
 * @return true if the string contains '*' or '?', false otherwise
 */
static bool hasWildcard(const std::string &mask)
{
	return mask.find_first_of("*?") != std::string::npos;
}

/**
 * @brief Normalizes a mask for banning users
 * @param ctx The request context
 * @param param The mask or nickname to normalize
 * @return The normalized mask or the original parameter if no user is found
 */
static std::string normalizeMask(RequestContext &ctx, const std::string &param)
{
	if (hasWildcard(param))
		return param;
	User *target = ctx.services.users().findByNick(param);
	if (!target)
		return param;
	return ctx.services.getUserPrefix(target);
}

/**
 * @brief Sends the current modes of a channel to the user
 * @param ctx The request context
 * @param ch The channel
 */
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

/**
 * @brief Executes a mode change for a channel
 * @param ctx The request context
 * @param m The mode character
 * @param add Whether to add or remove the mode
 * @param ch The channel
 * @param param The parameter for the mode
 * @param channelName The name of the channel
 * @return The parameter used for the mode change, or an empty string if none
 */
static std::string executeMode(RequestContext &ctx, char m, bool add, Channel *ch, const std::string &param, const std::string &channelName)
{
	switch (m)
	{
		case 'i':
			modeI(ch, add);
			return "";
		case 't':
			modeT(ch, add);
			return "";
		case 'm':
			modeM(ch, add);
			return "";
		case 'k':
			modeK(ch, add, param);
			return param;
		case 'l':
			if (!param.empty() && !isNumber(param))
			{
				ctx.services.sendResponse(ctx, ERR_INVALIDMODEPARAM(ctx.sender->getNickname(), channelName));
				return "";
			}
			modeL(ch, add, param.empty() ? -1 : std::atoi(param.c_str()));
			return param;
	case 'o':
	case 'v':
	case 'b':
	{
		User *target = ctx.services.users().findByNick(param);
		if (!target && m != 'b')
		{
			ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), param));
			return "";
		}
		if (m == 'b')
		{
			std::string mask = normalizeMask(ctx, param);
			if (add)
				ch->banMask(mask);
			else if (target)
				ch->unbanMatchingMask(ctx.services.getUserPrefix(target));
			else
				ch->unbanMask(mask);
			return mask;
		}
		else
			applyUserMode(ch, add, target, m);
		return param;
	}
		default:
			ctx.services.sendResponse(ctx, ERR_UNKNOWNMODE(ctx.sender->getNickname(), std::string(1, m)));
			return "";
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

		if (((m == 'k' || m == 'l' ) && add) || m == 'o' || m == 'v' || m == 'b')
		{
			if (p >= params.size())
			{
				ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "MODE"));
				return;
			}
			param = params[p++];
		}
		std::string sentParam = executeMode(ctx, m, add, ch, param, channelName);
		sendMode(ctx, channelName, add, m, sentParam);
		param.clear();
	}
}
