/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientRead.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:48 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:54:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "LineUtils.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>
#include <vector>
#include <map>

/** 
 * @brief Envía un mensaje de QUIT a todos los canales del usuario y lo desconecta.
 *
 * - Envía un mensaje de QUIT a todos los canales en los que el usuario está presente.
 * - Elimina al usuario de cada canal.
 * - Si un canal queda vacío, lo elimina del servidor.
 * - Finalmente, desconecta al usuario del servidor.
 *
 * @param server Puntero al servidor.
 * @param user Puntero al usuario a desconectar.
 */
static void sendQuitMessageToChannels(Server *server, User *user)
{
	if (!server || !user)
		return;
	const std::map<std::string, Channel *> &channels = user->getChannels();
	while (!channels.empty())
	{
		Channel *channel = user->getChannels().begin()->second;

		std::string quitMessage =
			":" + server->getServices().getUserPrefix(user) +
			" QUIT :Client disconnected\r\n";

		server->getServices().sendToChannel(channel, quitMessage, user);

		user->leaveChannel(channel);

		if (channel->isEmpty())
			server->getServices().channels().deleteChannel(channel->getName());
	}
}

/** 
 * @brief Desconecta un cliente del servidor.
 *
 * - Envía un mensaje de QUIT a todos los canales del usuario.
 * - Elimina al usuario del servidor.
 * - Elimina el descriptor de archivo del vector de descriptores.
 *
 * @param server Puntero al servidor.
 * @param user Puntero al usuario a desconectar.
 * @return true si el cliente fue desconectado, false en caso contrario.
 */
static bool disconnectClient(Server *server, User *user)
{
	if (!server || !user)
		return false;

	sendQuitMessageToChannels(server, user);
	std::cout << "[QUIT] - fd: " << user->getSocket() << "  : Disconnected" << std::endl;
	server->handleDisconnectionByFd(user->getSocket());
	return true;
}

/** 
 * @brief Procesa el buffer de entrada de un cliente.
 *
 * - Verifica si el usuario es válido.
 * - Extrae las líneas del buffer de entrada.
 * - Procesa cada línea como un comando del cliente.
 *
 * @param user Puntero al usuario cuyo buffer se va a procesar.
 */
void Server::processClientBuffer(User *user)
{
	if (!user)
		return;

	std::string &buffer = user->getInBuffer();
	std::vector<std::string> lines;
	popLines(buffer, lines);

	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		try
		{
			this->handleClientCommand(user, *it);
		}
		catch (const std::exception &e)
		{
			disconnectClient(this, user);
			break;
		}
	}
}

/** 
 * @brief Maneja los eventos de lectura para un cliente específico.
 *
 * - Verifica si el índice del cliente es válido.
 * - Comprueba si hay datos disponibles para leer.
 * - Lee los datos del cliente.
 * - Procesa el buffer de entrada del cliente.
 *
 * @param idx Índice del cliente en el vector de descriptores.
 * @return true si se manejó un evento de lectura, false en caso contrario.
 */
bool Server::handleClientRead(int idx)
{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;

	const struct pollfd &pfd = this->fds[idx];

	if (pfd.revents & (POLLHUP | POLLERR | POLLNVAL))
	{
		disconnectClient(this, getUserByFd(pfd.fd));
		return true;
	}

	if (!(pfd.revents & POLLIN))
		return false;

	int fd = pfd.fd;
	User *client = getUserByFd(fd);

	if (!client)
	{
		disconnectClient(this, getUserByFd(pfd.fd));
		return true;
	}
	
	std::string buffer(1024, '\0');

	ssize_t n = recv(fd, &buffer[0], buffer.size() - 1, 0);

	if (n <= 0)
	{
		disconnectClient(this, getUserByFd(pfd.fd));
		return true;
	}

	buffer[n] = '\0';

	client->getInBuffer().append(buffer, 0, static_cast<size_t>(n));

	processClientBuffer(client);

	return false;
}