/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:55 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:01:56 by agarcia          ###   ########.fr       */
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
#include "replies/Replies.hpp"

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

static void setUser(RequestContext &ctx, const std::string &username, const std::string &real)
{
	ctx.sender->setUsername(username);
	ctx.sender->setRealName(real);

}

void CmdUser::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;


	if (ctx.sender->isAuthenticated()) {
		ctx.services.sendResponse(ctx, ERR_ALREADYREGISTERED(ctx.sender->getNickname()));
		return;
	}
	
	if (ctx.rawLine.empty()) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "USER"));
		return;
	}
	std::string username, real;
	parseUserLine(ctx.rawLine, username, real);
	if (username.empty() || real.empty()) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "USER"));
		return;
	}
	setUser(ctx, username, real);
}
