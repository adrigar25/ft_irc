/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:55 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:39:13 by agarcia          ###   ########.fr       */
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

/**
 * @brief Parses the USER command parameters.
 * @param raw The raw command line.
 * @param outUsername The parsed username.
 * @param outReal The parsed real name.
 */
static void parseUserLine(const std::string &raw, std::string &outUsername, std::string &outReal)
{
	std::istringstream iss(raw);
	std::string username;
	std::string mode;
	std::string unused;
	std::string real;
	iss >> username >> mode >> unused;
	std::getline(iss, real);
	if (!real.empty() && real[0] == ' ') real.erase(0,1);
	if (!real.empty() && real[0] == ':') real.erase(0,1);
	outUsername = username;
	outReal = real;
}

/**
 * @brief Sets the user information for the sender.
 * @param ctx The request context.
 * @param username The username to set.
 * @param real The real name to set.
 */
static void setUser(RequestContext &ctx, const std::string &username, const std::string &real)
{
	ctx.sender->setUsername(username);
	ctx.sender->setRealName(real);

}

/**
 * @brief Executes the USER command.
 * - Handles the USER command for setting the user's username and real name.
 * - Validates the parameters and sends appropriate responses for errors.
 * @param ctx The request context.
 */
void CmdUser::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;


	if (ctx.sender->isAuthenticated()) {
		ctx.services.sendResponse(ctx, ERR_ALREADYREGISTERED(ctx.sender->getNickname()));
		return;
	}

	std::string username;
	std::string real;
	parseUserLine(ctx.rawLine, username, real);
	if (username.empty() || real.empty()) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "USER"));
		return;
	}
	setUser(ctx, username, real);
}
