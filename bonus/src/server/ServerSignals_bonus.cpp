/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSignals.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:17 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 17:01:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server_bonus.hpp"
#include <csignal>
#include <iostream>

/** 
 * @brief Manejador de señal para detener el servidor.
 *
 * - Obtiene la instancia del servidor.
 * - Si existe, llama al método `stopServer`.
 *
 * @param signum Número de la señal recibida.
 */
void handleSignalStopServer(int signum)
{
	Server *server = Server::getInstance();
	if (server) {
		(void)signum;
		server->stopServer();
	}
}

/** 
 * @brief Inicializa los manejadores de señales.
 *
 * - Configura el manejador para SIGINT.
 * - Configura el manejador para SIGTERM.
 * - Configura el manejador para SIGQUIT.
 */
void Server::initSignals()
{
	signal(SIGINT, handleSignalStopServer);
	signal(SIGTERM, handleSignalStopServer);
	signal(SIGQUIT, handleSignalStopServer);
}
