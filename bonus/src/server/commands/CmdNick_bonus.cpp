/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:20 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/28 18:34:34 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdNick_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "User_bonus.hpp"
#include "LineUtils_bonus.hpp"
#include "managers/UserManager_bonus.hpp"
#include "replies/Replies_bonus.hpp"
#include <string>
#include <cstring>

/**
 * @brief Validates the format of a nickname.
 * @param nick The nickname to validate.
 * @return True if the nickname is valid, false otherwise.
 */
static bool validateNickFormat(const std::string &nick)
{
	if (nick.empty())
		return false;
	if (std::isdigit((unsigned char)nick[0]))
		return false;
	for (size_t i = 0; i < nick.size(); ++i)
	{
		char c = nick[i];
		if (!std::isalnum((unsigned char)c) && c != '-' && c != '_' && c != '[' && c != ']' && c != '\\' && c != '`' && c != '{' && c != '}')
			return false;
	}
	return true;
}

/**
 * @brief Extracts the nickname from a raw line.
 * @param raw The raw line.
 * @return The extracted nickname.
 */
static std::string extractNick(const std::string &raw)
{
	std::string nick = trim(raw, " \r");
	if (nick.empty())
		return "";
	size_t spacePos = nick.find(' ');
	if (spacePos != std::string::npos)
		nick = nick.substr(0, spacePos);
	return trim(nick, " \r");
}

/**
 * @brief Checks if a nickname is available.
 * @param ctx The request context.
 * @param nick The nickname to check.
 * @return True if the nickname is available, false otherwise.
 */
static bool nickAvailable(RequestContext &ctx, const std::string &nick)
{
	const User *existing = ctx.services.users().findByNick(nick);
	if (existing && existing != ctx.sender)
	{
		ctx.services.sendResponse(ctx, ERR_NICKNAMEINUSE(ctx.sender->getNickname(), nick));
		return false;
	}
	return true;
}

/**
 * @brief Executes the NICK command.
 *  - Handles the NICK command for changing a user's nickname.
 *  - Validates the nickname format and availability.
 *  - Sends appropriate responses for errors and successful nickname changes.
 * @param ctx The request context.
 */
void CmdNick::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;
	std::string nick = extractNick(ctx.rawLine);
	std::string oldPrefix = nick + "!" + (ctx.sender->getUsername().empty() ? "user" : ctx.sender->getUsername()) + "@" + ctx.services.getServerName();

	if (ctx.sender->isAuthenticated() && !ctx.sender->getNickname().empty())
		oldPrefix = ctx.services.getUserPrefix(ctx.sender);

	if (nick.empty())
	{
		ctx.services.sendResponse(ctx, ERR_NONICKNAMEGIVEN(ctx.sender->getNickname()));
		return;
	}

	if (!validateNickFormat(nick))
	{
		ctx.services.sendResponse(ctx, ERR_ERRONEUSNICKNAME(ctx.sender->getNickname(), nick));
		return;
	}

	if (!nickAvailable(ctx, nick))
		return;
	ctx.sender->setNickname(nick);
	ctx.services.sendToUser(ctx.sender, ":" + oldPrefix + " " + RPL_NICK(nick));
}
