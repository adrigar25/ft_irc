/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerAccept.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:09 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 17:08:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "SocketUtils.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/** 
 * @brief Maneja la aceptación de nuevas conexiones.
 *
 * - Entrada en un bucle infinito para aceptar conexiones.
 * - Acepta la conexión entrante.
 * - Configura el socket como no bloqueante.
 * - Agrega el nuevo descriptor de archivo al vector de descriptores.
 * - Crea un nuevo usuario para el cliente.
 *
 */
void Server::handleNewConnection()
{
	while (true)
	{
		struct sockaddr_in clientAddress;
		socklen_t clientAddressLen = sizeof(clientAddress);
		int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
		if (newSocket < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
			throw IrcException(IRC_ERR_ACCEPTING_CONNECTION, std::string("accept failed: ") + strerror(errno));
		}

		setSocketNonBlocking(newSocket);
		pushPollFd(newSocket, POLLIN);

		User* newUser = new User(newSocket, "*");
		newUser->setHost(inet_ntoa(clientAddress.sin_addr));
		addUser(newUser);
		std::cout << "Accepted client fd " << newSocket << std::endl;
	}
}

/** 
 * @brief Maneja la desconexión de un cliente por su índice en el vector de descriptores.
 *
 * - Verifica si el índice es válido.
 * - Elimina al usuario del servidor.
 * - Elimina el descriptor de archivo del vector.
 *
 * @param idx Índice del cliente en el vector de descriptores.
 */
void Server::handleDisconnectionByIndex(int idx)
{
	if (idx <= 0 || idx >= (int)this->fds.size()) {
		std::cerr << "handleDisconnectionByIndex: invalid idx " << idx << std::endl;
		return;
	}
	int fd = this->fds[idx].fd;

	deleteUser(fd);
	this->fds.erase(this->fds.begin() + idx);
}

/** 
 * @brief Maneja la desconexión de un cliente por su descriptor de archivo.
 *
 * - Busca el descriptor de archivo en el vector de descriptores.
 * - Llama a `handleDisconnectionByIndex` con el índice encontrado.
 *
 * @param fd Descriptor de archivo del cliente.
 */
void Server::handleDisconnectionByFd(int fd)
{
	for (size_t i = 1; i < this->fds.size(); ++i) {
		if (this->fds[i].fd == fd) {
			handleDisconnectionByIndex(static_cast<int>(i));
			return;
		}
	}
	std::cerr << "handleDisconnectionByFd: fd not found " << fd << std::endl;
}
