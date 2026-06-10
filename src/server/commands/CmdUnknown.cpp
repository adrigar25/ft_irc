/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUnknown.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:09 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:38:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdUnknown.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>

void CmdUnknown::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    ctx.services.sendToUser(ctx.sender, std::string("421 ") + ctx.rawLine + " :Unknown command");
}
#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleUnknownCommand(User *user, const std::string &command)
{
    sendToUser(user, std::string("421 ") + command + " :Unknown command");
    return true;
}
