/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:30 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:20:31 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPass.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <string>
#include <cstring>

#include "LineUtils.hpp"
#include "replies/Replies.hpp"

static bool applyPassword(RequestContext &ctx, const std::string &pass)
{
	if(ctx.sender->isPassSet()) {
		ctx.services.sendResponse(ctx, ERR_ALREADYREGISTERED(ctx.sender->getNickname()));
		return false;
	}
	Server* srv = ctx.services.getServer();
	if (srv && srv->validatePassword(pass)) {
		ctx.sender->setPass(true);
		return true;
	}
	ctx.services.sendResponse(ctx, ERR_PASSWDMISMATCH(ctx.sender->getNickname()));
	return false;
}

void CmdPass::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string params = trim(ctx.rawLine, " \r");

	if (params.empty()) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "PASS"));
		return;
	}
	applyPassword(ctx, params);
}
