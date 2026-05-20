/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_users.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:30 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 17:05:49 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

/**
 * @brief Añade un `User` al mapa `users` indexado por su descriptor.
 *
 * Lanza `userAlreadyExistsException` si ya existe un usuario con ese fd.
 * @param user Puntero al `User` a añadir.
 */
void Server::addUser(User* user)
{
    int fd = user->getSocket();
    if(this->users.find(fd) != this->users.end())
       throw userAlreadyExistsException(std::string("User already exists: fd ") + std::to_string(fd));
    this->users.insert(std::make_pair(fd, user));
}


/**
 * @brief Elimina y libera un usuario identificado por `fd`.
 *
 * - Elimina al usuario de todos los canales existentes llamando a
 *   `Channel::deleteUser` y captura cualquier excepción durante la
 *   eliminación de canales para no abortar la operación.
 * - Borra la entrada del mapa `users` y libera la memoria del objeto.
 *
 * @param fd Descriptor del usuario a eliminar.
 */
void Server::deleteUser(int fd)
{
    std::map<int, User*>::iterator it = this->users.find(fd);

    if (it != this->users.end()) {
        User *user = it->second;
        for (std::map<std::string, Channel*>::iterator cit = this->channels.begin(); cit != this->channels.end(); ++cit) {
            try {
                if (cit->second) cit->second->deleteUser(user);
            } catch (const std::exception &e) {
                std::cerr << "ERROR: error removing user from channel " << cit->first << ": " << e.what() << std::endl;
            }
        }
        this->users.erase(it);
        delete user;
    }
}

/* getChannel moved to src/server_channels.cpp */

/**
 * @brief Busca y devuelve un `User*` por su `nickname`.
 *
 * Recorre el mapa `users` y compara `getNickname()`; retorna `NULL` si
 * no se encuentra ningún usuario con ese nickname.
 *
 * @param nickname Nickname buscado.
 * @return Puntero a `User` o `NULL` si no existe.
 */
User* Server::getUserByNickname(const std::string &nickname)
{
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it) {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

/**
 * @brief Busca y devuelve un `User*` por su descriptor de socket `fd`.
 *
 * Recorre el mapa `users` buscando la entrada con clave `fd`. Retorna
 * el puntero a `User` si se encuentra, o `NULL` si no existe.
 *
 * @param fd Descriptor de socket del usuario buscado.
 * @return Puntero a `User` o `NULL` si no existe.
 */
User* Server::getUserByFd(int fd)
{
    std::map<int, User*>::iterator it = this->users.find(fd);
    if (it == this->users.end())
        return NULL;
    return it->second;
}