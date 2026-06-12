/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:50 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:20:41 by agarcia          ###   ########.fr       */
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

static std::string extractNick(const std::string &raw)
{
    std::string nick = trim(raw, " \r");
    if (nick.empty())
        return "";
    size_t spacePos = nick.find(' ');
    if (spacePos != std::string::npos)
        nick = nick.substr(0, spacePos);
    return nick;
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
