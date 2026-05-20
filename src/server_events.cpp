/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_events.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:25 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 16:59:30 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>


/**
 * @brief Envuelve la llamada a `poll` sobre el vector `fds` del servidor.
 *
 * @return Número de descriptores listos (>0), 0 si la llamada fue
 * interrumpida por una señal (`EINTR`), o -1 en caso de error.
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
 * @brief Recorre los descriptores de cliente y procesa lectura y errores.
 *
 * Itera sobre `this->fds` (desde 1 para saltarse el `serverSocket`) y
 * llama a `handleClientRead` y `handleClientErrorEvents`. Si alguno de los
 * handlers devuelve `true` (desconexión), decrementa el índice para mantener
 * la coherencia del bucle tras la eliminación del descriptor.
 */
void Server::handleClientEvents()
{
    for (int i = 1; i < (int)this->fds.size(); ++i)
    {
        if (handleClientWrite(i)) { i--; continue; }
        if (handleClientRead(i)) { i--; continue; }
        if (handleClientError(i)) { i--; continue; }
    }
}


/*
 * @brief Maneja los eventos del servidor, incluyendo conexiones nuevas y eventos de clientes.
 * - En el bucle principal, llama a `performPoll` para esperar eventos.
 * - Si `performPoll` devuelve >0, verifica si el primer descriptor (`serverSocket`) tiene `POLLIN` para aceptar nuevas conexiones.
 * - Luego llama a `handleClientEvents` para procesar eventos de lectura y errores en los clientes conectados.
 * - Captura y registra cualquier excepción que ocurra durante la aceptación de conexiones o el procesamiento de eventos de clientes, sin abortar el
 */
void Server::handleEvents()
{
    int ready = 0;
    while (true)
    {
        if (this->fds.size() == 0)
            continue;
        ready = performPoll();
        if (ready == 0)
            continue;
        if (ready < 0)
            break;
        if (!this->fds.empty() && (this->fds[0].revents & POLLIN))
        {
            try {
                handleNewConnection();
            } catch (const std::exception &e) {
                std::cerr << "Accept error: " << e.what() << std::endl;
            }
        }

        try {
            handleClientEvents();
        } catch (const std::exception &e) {
            std::cerr << "Error processing client events: " << e.what() << std::endl;
        }
    }
}
