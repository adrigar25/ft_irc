/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/28 18:43:24 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Services_bonus.hpp"
#include "Server_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "replies/Replies_bonus.hpp"

Services::Services(Server *srv) : server(srv) {}

Services::~Services() {}

UserManager &Services::users() { return userManager; }
ChannelManager &Services::channels() { return channelManager; }
const UserManager &Services::Users() { return userManager; }
const ChannelManager &Services::channels() const { return channelManager; }

Server *Services::getServer() const { return server; }

/**
 * @brief Envía `message` a `user` delegando la lógica de I/O al servidor.
 *
 * @param user Puntero al usuario destinatario.
 * @param message Mensaje a enviar.
 */
void Services::sendToUser(User *user, const std::string &message)
{
	if (server)
		server->sendToUser(user, message);
}

/**
 * @brief Envía `message` a los usuarios de `channel`, excluyendo `exclude`.
 *
 * @param channel Canal destino.
 * @param message Mensaje a enviar.
 * @param exclude Usuario a excluir (opcional).
 */
void Services::sendToChannel(Channel *channel, const std::string &message, const User *exclude)
{
	if (server)
		server->sendToChannel(channel, message, exclude);
}

/**
 * @brief Obtiene el nombre del servidor.
 * @return El nombre del servidor.
 */
std::string Services::getServerName() const
{
	if (server)
		return server->getHostname();
	return std::string("localhost");
}

/**
 * @brief Obtiene el prefijo para un usuario.
 * @param user Puntero al usuario.
 * @return El prefijo del usuario.
 */
std::string Services::getUserPrefix(const User *user) const
{
	return user->getNickname() + "!" + user->getUsername() + "@" + user->getHost();
}

/**
 * @brief Obtiene el prefijo para el servidor.
 * @return El prefijo del servidor.
 */
std::string Services::getServerPrefix() const
{
	std::string prefix = ":";
	if (server)
		prefix += server->getHostname();
	else
		prefix += "localhost";
	return prefix;
}

/**
 * @brief Envía una respuesta al usuario.
 * @param ctx Contexto de la solicitud.
 * @param reply Respuesta a enviar.
 */
void Services::sendResponse(RequestContext &ctx, const std::string &reply)
{
	std::string response = getServerPrefix() + " " + reply;
	ctx.services.sendToUser(ctx.sender, response);
}

/**
 * @brief Envía la lista de nombres de usuarios en un canal.
 * @param ctx Contexto de la solicitud.
 * @param target Usuario destinatario.
 * @param channel Canal.
 */
void Services::sendNamesList(RequestContext &ctx, const User *target, Channel *channel)
{
	std::string serverName = ctx.services.getServerName();
	const std::map<int, User *> &usersMap = channel->getUsers();
	std::string namesList;
	for (std::map<int, User *>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit)
	{
		if (!namesList.empty())
			namesList += " ";
		if (channel->isUserOperator(uit->second))
			namesList += "@";
		if (channel->isUserVoice(uit->second))
			namesList += "+";
		namesList += uit->second->getNickname();
	}

	sendResponse(ctx, RPL_NAMREPLY(target->getNickname(), "=", channel->getName(), namesList));
	sendResponse(ctx, RPL_ENDOFNAMES(target->getNickname(), channel->getName()));
}
