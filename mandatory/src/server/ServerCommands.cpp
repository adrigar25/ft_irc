/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:21:47 by adriescr          #+#    #+#             */
/*   Updated: 2026/07/01 14:04:05 by agarcia          ###   ########.fr       */
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
#include "commands/CmdMsg.hpp"
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
#include "LineUtils.hpp"

/**
 * @brief Obtiene el despachador de comandos.
 * @return Referencia constante al despachador de comandos.
 */
static const CommandDispatcher &getDispatcher()
{
	static CommandDispatcher dispatcher;
	static bool inited = false;
	if (!inited)
	{
		dispatcher.registerHandler("PASS", new CmdPass());
		dispatcher.registerHandler("NICK", new CmdNick());
		dispatcher.registerHandler("USER", new CmdUser());
		dispatcher.registerHandler("JOIN", new CmdJoin());
		dispatcher.registerHandler("PART", new CmdPart());
		dispatcher.registerHandler("PRIVMSG", new CmdMsg(false));
		dispatcher.registerHandler("NOTICE", new CmdMsg(true));
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

/**
 * @brief Verifica si el usuario está autenticado.
 * @param ctx Contexto de la solicitud.
 */
static void checkAuthentication(RequestContext &ctx)
{
	if (ctx.sender->isPassSet() && ctx.sender->isNickSet() && ctx.sender->isUserSet())
	{
		ctx.sender->setAuthenticated(true);
		std::string serverName = ctx.services.getServerName();
		ctx.services.sendResponse(ctx, RPL_WELCOME(ctx.sender->getNickname(), ctx.sender->getNickname(), ctx.sender->getUsername(), serverName));
		ctx.services.sendResponse(ctx, RPL_YOURHOST(ctx.sender->getNickname(), serverName));
		ctx.services.sendResponse(ctx, RPL_CREATED(ctx.sender->getNickname(), serverName));
		ctx.services.sendResponse(ctx, RPL_MYINFO(ctx.sender->getNickname(), serverName));
	}
}

/**
 * @brief Maneja comandos desconocidos.
 *
 * - Envía un mensaje de error al usuario indicando que el comando no es reconocido.
 * @param user Usuario que envió el comando desconocido.
 * @param command Comando desconocido.
 */
void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{

	const CommandDispatcher &dispatcher = getDispatcher();
	RequestContext ctx(this->services, user, args);

	if (!dispatcher.hasHandler(command))
	{
		if (command.empty())
			return;
		handleUnknownCommand(user, command);
		return;
	}

	if (!user->isAuthenticated())
	{

		if (!user->isPassSet() && command != "PASS" && command != "CAP" && command != "PING")
		{
			sendToUser(user, ERR_NOTREGISTERED(user->getNickname()));
			return;
		}

		if (command != "PASS" && command != "NICK" && command != "USER" && command != "CAP" && command != "PING")
		{
			sendToUser(user, ERR_NOTREGISTERED(user->getNickname()));
			return;
		}
	}

	dispatcher.dispatch(command, ctx);

	if (command == "QUIT")
		return;

	if (!user->isAuthenticated())
		checkAuthentication(ctx);
}

/**
 * @brief Maneja comandos enviados por el cliente.
 *
 * - Verifica si el usuario está autenticado.
 * - Ejecuta el comando correspondiente.
 * @param user Usuario que envió el comando.
 * @param commandLine Línea del comando recibido.
 */
void Server::handleClientCommand(User *user, const std::string &commandLine)
{
	if (!user)
	{
		std::cerr << "handleClientCommand: null user for command: " << commandLine << std::endl;
		return;
	}
	if (commandLine.empty())
		return;
	std::vector<std::string> tokens = split(commandLine, ' ');
	std::string cmd = tokens[0];
	std::string args = tokens.size() > 1 ? commandLine.substr(cmd.size() + 1) : "";

	std::cout << "[" << cmd << "] - fd: " << user->getSocket();
	if (!args.empty())
		std::cout << " - args: " << args;
	std::cout << std::endl;

	executeCommand(user, cmd, args);
}