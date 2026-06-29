/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEvents.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 17:05:43 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>


/** 
 * @brief Realiza una llamada a `poll` para verificar los eventos en los sockets.
 *
 * - Obtiene el número de descriptores de archivo.
 * - Llama a `poll` con un tiempo de espera infinito.
 * - Maneja los errores de `poll`.
 *
 * @return El número de descriptores listos o -1 en caso de error.
 */
int Server::performPoll()
{
	int nfds = this->fds.size();
	int ready = poll(this->fds.data(), nfds, -1);
	if (ready < 0) {
		if (errno == EINTR) return 0;
		std::cerr << "poll error: " << strerror(errno) << std::endl;
		return -1;
	}
	return ready;
}

/** 
 * @brief Maneja los eventos de los clientes.
 *
 * - Itera sobre los sockets de los clientes.
 * - Llama a las funciones correspondientes para manejar escritura, lectura y errores.
 */
void Server::handleClientEvents()
{
	for (int i = 1; i < (int)this->fds.size(); ++i)
	{
		if (handleClientWrite(i) || handleClientRead(i) || handleClientError(i))
		{
			i--;
			continue;
		}
	}
}

/** 
 * @brief Maneja los eventos del servidor.
 *
 * - Realiza una llamada a `performPoll` para verificar los eventos en los sockets.
 * - Si hay eventos disponibles, llama a `handleNewConnection` o `handleClientEvents` según el tipo de evento.
 * - Continúa hasta que el servidor deje de correr.
 */
void Server::handleEvents()
{
	int ready = 0;

	while (this->running)
	{
		if (this->fds.empty())
		{
			usleep(100000);
			continue;
		}
		ready = performPoll();
		if (ready < 0)
			break;

		if (ready == 0)
			continue;

		if (!this->fds.empty() && (this->fds[0].revents & POLLIN))
			handleNewConnection();

		handleClientEvents();
	}
	cleanup();
}
