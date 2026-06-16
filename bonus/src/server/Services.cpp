/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:25 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Services.hpp"
#include "Server.hpp"
#include "RequestContext.hpp"
#include "replies/Replies.hpp"

Services::Services(Server* srv): server(srv), userManager(), channelManager() {}

Services::~Services() {}

UserManager& Services::users() { return userManager; }
ChannelManager& Services::channels() { return channelManager; }
const UserManager& Services::users() const { return userManager; }
const ChannelManager& Services::channels() const { return channelManager; }

Server* Services::getServer() const { return server; }

/**
 * @brief Envía `message` a `user` delegando la lógica de I/O al servidor.
 *
 * @param user Puntero al usuario destinatario.
 * @param message Mensaje a enviar.
 */
void Services::sendToUser(User* user, const std::string &message) {
	if (server) server->sendMessageToUser(user, message);
}

/**
 * @brief Envía `message` a los usuarios de `channel`, excluyendo `exclude`.
 *
 * @param channel Canal destino.
 * @param message Mensaje a enviar.
 * @param exclude Usuario a excluir (opcional).
 */
void Services::sendToChannel(Channel* channel, const std::string &message, User* exclude) {
	if (server) server->sendMessageToChannel(channel, message, exclude);
}
std::string Services::getServerName() const {
	if (server)
		return server->getHostname();
	return std::string("localhost");
}

std::string Services::getUserPrefix(User* user) const
{
	   return user->getNickname() + "!" + user->getUsername() + "@" + getServerName();
}

std::string Services::getServerPrefix() const
{
	std::string prefix = ":";
	if (server)
		prefix += server->getHostname();
	else
		prefix += "localhost";
	return prefix;
}

void Services::sendResponse(RequestContext &ctx, const std::string &reply)
{
	std::string response = getServerPrefix() + " " + reply;
	ctx.services.sendToUser(ctx.sender, response);
}

void Services::sendNamesList(RequestContext &ctx, User *target, Channel *channel)
{
	std::string serverName = ctx.services.getServerName();
	const std::map<int, User*>& usersMap = channel->getUsers();
	std::string namesList;
	for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
		if (!namesList.empty())
			namesList += " ";
		if (channel->isUserOperator(uit->second))
			namesList += "@";
		if(channel->isUserVoice(uit->second))
			namesList += "+";
		namesList += uit->second->getNickname();
	}

	sendResponse(ctx, RPL_NAMREPLY(target->getNickname(), "=", channel->getName(), namesList));
	sendResponse(ctx, RPL_ENDOFNAMES(target->getNickname(), channel->getName()));
}
