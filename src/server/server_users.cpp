#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>
#include <sstream>

/**
 * @brief Añade un `User` al mapa `users` indexado por su descriptor.
 *
 * Lanza `userAlreadyExistsException` si ya existe un usuario con ese fd.
 * @param user Puntero al `User` a añadir.
 */
void Server::addUser(User* user)
{
    this->services.users().add(user);
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
    User* user = this->services.users().findByFd(fd);
    if (!user) return;
    const std::map<std::string, Channel*>& channels = this->services.channels().getAll();
    for (std::map<std::string, Channel*>::const_iterator cit = channels.begin(); cit != channels.end(); ++cit) {
        try {
            if (cit->second) cit->second->deleteUser(user);
        } catch (const std::exception &e) {
            std::cerr << "ERROR: error removing user from channel " << cit->first << ": " << e.what() << std::endl;
        }
    }
    this->services.users().remove(fd);
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
    return this->services.users().findByFd(fd);
}
