/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:39 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:00:40 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <utility>
#include "CommandDispatcher.hpp"
#include "commands/CmdPrivmsg.hpp"
#include "commands/CmdPass.hpp"
#include "commands/CmdNick.hpp"
#include "commands/CmdUser.hpp"
#include "commands/CmdJoin.hpp"
#include "commands/CmdPart.hpp"
#include "commands/CmdPing.hpp"
#include "commands/CmdQuit.hpp"
#include "commands/CmdList.hpp"
#include "commands/CmdInvite.hpp"
#include "commands/CmdKick.hpp"
#include "commands/CmdMode.hpp"
#include "commands/CmdTopic.hpp"
#include "commands/CmdCap.hpp"
#include "commands/CmdNames.hpp"
#include "RequestContext.hpp"
#include "replies/Replies.hpp"

static CommandDispatcher& getDispatcher()
{
	static CommandDispatcher dispatcher;
	static bool inited = false;
	if (!inited) {
		dispatcher.registerHandler("PASS", new CmdPass());
		dispatcher.registerHandler("NICK", new CmdNick());
		dispatcher.registerHandler("USER", new CmdUser());
		dispatcher.registerHandler("JOIN", new CmdJoin());
		dispatcher.registerHandler("PART", new CmdPart());
		dispatcher.registerHandler("PRIVMSG", new CmdPrivmsg());
		dispatcher.registerHandler("QUIT", new CmdQuit());
		dispatcher.registerHandler("PING", new CmdPing());
		dispatcher.registerHandler("LIST", new CmdList());
		dispatcher.registerHandler("INVITE", new CmdInvite());
		dispatcher.registerHandler("KICK", new CmdKick());
		dispatcher.registerHandler("MODE", new CmdMode());
		dispatcher.registerHandler("TOPIC", new CmdTopic());
		dispatcher.registerHandler("CAP", new CmdCap());
		dispatcher.registerHandler("NAMES", new CmdNames());
		inited = true;
	}
	return dispatcher;
}

static void checkAuthentication(RequestContext &ctx)
{
	if (ctx.sender->isPassSet() && ctx.sender->isNickSet() && ctx.sender->isUserSet()) {
		ctx.sender->setAuthenticated(true);
		std::string serverName = ctx.services.getServerName();
		ctx.services.sendResponse(ctx, RPL_WELCOME(ctx.sender->getNickname(), ctx.sender->getNickname(), ctx.sender->getUsername(), serverName));
		ctx.services.sendResponse(ctx, RPL_YOURHOST(ctx.sender->getNickname(), serverName));
		ctx.services.sendResponse(ctx, RPL_CREATED(ctx.sender->getNickname(), serverName));
		ctx.services.sendResponse(ctx, RPL_MYINFO(ctx.sender->getNickname(), serverName));
	}
}
void Server::handleClientCommand(User *user, const std::string &commandLine)
{
	if (!user) {
		std::cerr << "handleClientCommand: null user for command: " << commandLine << std::endl;
		return;
	}
	if (commandLine.empty())
		return;
	size_t sp = commandLine.find(' ');
	std::string cmd = commandLine;
	std::string args = "";
	if (sp != std::string::npos) {
		cmd = commandLine.substr(0, sp);
		if (sp + 1 < commandLine.size())
			args = commandLine.substr(sp + 1);
	}
	std::cout << "[" << cmd << "]";
	if (!args.empty())
		std::cout << " " << args;
	std::cout << std::endl;
	executeCommand(user, cmd, args);
}

void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{

	CommandDispatcher& dispatcher = getDispatcher();

	if (!user)
		return;

	if (!dispatcher.hasHandler(command))
	{
		if (command.empty())
			return;
		handleUnknownCommand(user, command);
		return;
	}

	if (!user->isAuthenticated())
	{
		if (command != "PASS" && command != "NICK" && command != "USER" && command != "CAP" && command != "PING" && command != "PONG")
		{
			sendToUser(user, ERR_NOTREGISTERED(user->getNickname()));
			return;
		}
	}

	RequestContext ctx(this->services, user, args);
	dispatcher.dispatch(command, ctx);
	if(!user->isAuthenticated())
		checkAuthentication(ctx);
}
