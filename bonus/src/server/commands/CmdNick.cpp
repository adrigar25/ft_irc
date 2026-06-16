/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:20 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:20:21 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdNick.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include "LineUtils.hpp"
#include "managers/UserManager.hpp"
#include "replies/Replies.hpp"
#include <string>
#include <cstring>

static bool validateNickFormat(const std::string &nick)
{
	if (nick.empty())
		return false;
	if (std::isdigit((unsigned char)nick[0]))
		return false;
	for (size_t i = 0; i < nick.size(); ++i) {
		char c = nick[i];
		if (!std::isalnum((unsigned char)c) && c != '-' && c != '_' && c != '[' && c != ']' && c != '\\' && c != '`' && c != '{' && c != '}')
			return false;
	}
	return true;
}

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

static bool nickAvailable(RequestContext &ctx, const std::string &nick)
{
	User* existing = ctx.services.users().findByNick(nick);
	if (existing && existing != ctx.sender) {
		ctx.services.sendResponse(ctx, ERR_NICKNAMEINUSE(ctx.sender->getNickname(), nick));
		return false;
	}
	return true;
}


void CmdNick::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string nick = extractNick(ctx.rawLine);

	if (nick.empty()) {
		ctx.services.sendResponse(ctx, ERR_NONICKNAMEGIVEN(ctx.sender->getNickname()));
		return;
	}

	if (!validateNickFormat(nick)) {
		ctx.services.sendResponse(ctx, ERR_ERRONEUSNICKNAME(ctx.sender->getNickname(), nick));
		return;
	}

	if (!nickAvailable(ctx, nick)) return;
		ctx.sender->setNickname(nick);
}
