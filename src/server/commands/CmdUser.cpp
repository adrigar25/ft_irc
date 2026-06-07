/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:11 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 15:02:34 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdUser.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstring>

static void parseUserLine(const std::string &raw, std::string &outUsername, std::string &outReal)
{
    std::istringstream iss(raw);
    std::string username, mode, unused, real;
    iss >> username >> mode >> unused;
    std::getline(iss, real);
    if (!real.empty() && real[0] == ' ') real.erase(0,1);
    if (!real.empty() && real[0] == ':') real.erase(0,1);
    outUsername = username;
    outReal = real;
}

static void setUserAndMaybeWelcome(RequestContext &ctx, const std::string &username, const std::string &real)
{
    (void)real;
    ctx.sender->setUsername(username);
    ctx.services.sendToUser(ctx.sender, std::string("Username set"));

    if (ctx.sender->isPassSet() && ctx.sender->isNickSet() && ctx.sender->isUserSet()) {
        ctx.sender->setAuthenticated(true);
        std::string serverName = ctx.services.getServerName();
        std::string welcome = std::string(":") + serverName + " 001 " + ctx.sender->getNickname() + " :Welcome to " + serverName;
        ctx.services.sendToUser(ctx.sender, welcome);
    }
}

void CmdUser::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    if (ctx.rawLine.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 USER :Not enough parameters"));
        return;
    }
    std::string username, real;
    parseUserLine(ctx.rawLine, username, real);
    setUserAndMaybeWelcome(ctx, username, real);
}
