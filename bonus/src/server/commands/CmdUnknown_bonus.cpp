/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUnknown.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:00 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 19:12:40 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdUnknown_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include <string>
#include "replies/Replies_bonus.hpp"
#include "Server_bonus.hpp"
#include "User_bonus.hpp"
#include <string>

void CmdUnknown::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	if (ctx.rawLine.empty())
		return;
	ctx.services.sendResponse(ctx, ERR_UNKNOWNCOMMAND(ctx.sender->getNickname(), ctx.rawLine));
}

bool Server::handleUnknownCommand(User *user, const std::string &command)
{
	if (command.empty())
		return false;
	std::string reply = ERR_UNKNOWNCOMMAND(user->getNickname(), command);
	sendToUser(user, std::string(":") + this->services.getServerName() + " " + reply);
	return true;
}
