/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:50 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 15:02:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdNick.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include "managers/UserManager.hpp"
#include <string>
#include <cstring>

static std::string extractNick(const std::string &raw)
{
    std::string nick = raw;
    if (!nick.empty() && nick[0] == ' ') nick.erase(0, 1);
    if (!nick.empty() && nick[nick.size() - 1] == '\r') nick.erase(nick.size() - 1, 1);
    return nick;
}

static bool nickAvailable(RequestContext &ctx, const std::string &nick)
{
    User* existing = ctx.services.users().findByNick(nick);
    if (existing && existing != ctx.sender) {
        ctx.services.sendToUser(ctx.sender, std::string("433 NICK :Nickname is already in use"));
        return false;
    }
    return true;
}


void CmdNick::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string nick = extractNick(ctx.rawLine);

    if (nick.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("431 NICK :No nickname given"));
        return;
    }

    if (!nickAvailable(ctx, nick)) return; 
        ctx.sender->setNickname(nick);
}
