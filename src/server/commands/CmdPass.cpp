/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:57 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:21:17 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPass.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <string>
#include <cstring>

#include "LineUtils.hpp"

static bool applyPassword(RequestContext &ctx, const std::string &pass)
{
    if(ctx.sender->isPassSet()) {
        ctx.services.sendToUser(ctx.sender, std::string("462 PASS :You may not reregister"));
        return false;
    }
    Server* srv = ctx.services.getServer();
    if (srv && srv->validatePassword(pass)) {
        ctx.sender->setPass(true);
        ctx.services.sendToUser(ctx.sender, std::string("Password accepted"));
        return true;
    }
    ctx.services.sendToUser(ctx.sender, std::string("Invalid password"));
    return false;
}

void CmdPass::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string params = trim(ctx.rawLine, " \r");

    if (params.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 PASS :Not enough parameters"));
        return;
    }
    applyPassword(ctx, params);
}
