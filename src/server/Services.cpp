/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:39:01 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 12:47:27 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Services.hpp"
#include "Server.hpp"

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

void Services::sendResponse(RequestContext &ctx, const std::string &code, const std::string &message)
{
    std::string serverName = ctx.services.getServerName();
    std::string response = std::string(":") + serverName + " " + code + " " + ctx.sender->getNickname() + " " + message;
    ctx.services.sendToUser(ctx.sender, response);
}