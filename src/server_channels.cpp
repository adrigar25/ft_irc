/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_channels.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:21 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:35:54 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
/**
 * @brief Crea un nuevo canal con el nombre `name` y lo registra en el servidor.
 *
 * Lanza `channelAlreadyExistsException` si ya existe un canal con ese nombre.
 * Reserva dinámicamente un `Channel` y lo inserta en `this->channels`.
 *
 * @param name Nombre del canal a crear.
 * @param creator Usuario que crea el canal (owner inicial).
 * @throws channelAlreadyExistsException Si el canal ya existe.
 */
void Server::createChannel(std::string name, User *creator)
{
    if (this->channels.find(name) != this->channels.end())
        throw channelAlreadyExistsException(std::string("Channel already exists: ") + name);

    Channel *ch = new Channel(name, creator);
    this->channels.insert(std::make_pair(name, ch));
}

/**
 * @brief Obtiene un puntero al `Channel` con nombre `name`.
 *
 * Retorna un puntero al canal si existe, o `NULL` si no se encuentra.
 *
 * @param name Nombre del canal buscado.
 * @return `Channel*` puntero al canal o `NULL` si no existe.
 */
Channel* Server::getChannel(const std::string &name)
{
    std::map<std::string, Channel*>::iterator it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}
