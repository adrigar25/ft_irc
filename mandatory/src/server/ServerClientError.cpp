/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:54 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 17:10:17 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <poll.h>
#include <sys/socket.h>

/** 
 * @brief Maneja los eventos de error para un cliente específico.
 *
 * - Verifica si el índice del cliente es válido.
 * - Comprueba si hay errores en el descriptor de archivo.
 * - Desconecta al cliente si hay errores.
 *
 * @param idx Índice del cliente en el vector de descriptores.
 * @return true si se manejó un evento de error, false en caso contrario.
 */
bool Server::handleClientError(int idx)
{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;

	if (this->fds[idx].revents & (POLLHUP | POLLERR | POLLNVAL))
	{
		handleDisconnectionByIndex(idx);
		return true;
	}

	return false;
}
