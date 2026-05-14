/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_accept.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:19 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:12:57 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/**
 * @brief Establece el descriptor `fd` en modo no bloqueante.
 *
 * Obtiene las banderas actuales con `fcntl(F_GETFL)` y añade `O_NONBLOCK`
 * usando `fcntl(F_SETFL)`. Esto permite que operaciones de lectura/escritura
 * sobre sockets no queden bloqueadas esperando datos.
 *
 * @param fd Descriptor de archivo (socket) al que aplicar la modificación.
 *
 * @throws errorSettingNonblockingException Si falla `fcntl` al leer o escribir
 *         las banderas; el mensaje incluye la descripción de `errno`.
 *
 * @note No cierra el descriptor en caso de error; el llamador debe manejar
 *       la limpieza si es necesario. No es intrínsecamente seguro para
 *       concurrencia; sincronizar externamente si varios hilos usan el mismo fd.
 */
void Server::setSocketNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::string msg = std::string("fcntl F_GETFL: ") + strerror(errno);
        perror("fcntl F_GETFL");
        throw errorSettingNonblockingException(msg);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::string msg = std::string("fcntl F_SETFL: ") + strerror(errno);
        perror("fcntl F_SETFL");
        throw errorSettingNonblockingException(msg);
    }
}

/**
 * @brief Activa la bandera `FD_CLOEXEC` para el descriptor `fd`.
 *
 * Evita que el descriptor sea heredado por procesos hijos tras una llamada a
 * `exec`. Usa `fcntl(F_GETFD)` y `fcntl(F_SETFD)` para leer y modificar
 * las banderas de descriptor.
 *
 * @param fd Descriptor de archivo a modificar.
 * @throws errorSettingCloexecException Si `fcntl` falla al obtener o establecer
 *         las banderas (el mensaje incluirá la descripción de `errno`).
 */
void Server::setSocketCloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        throw errorSettingCloexecException(std::string("fcntl F_GETFD: ") + strerror(errno));
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
        throw errorSettingCloexecException(std::string("fcntl F_SETFD: ") + strerror(errno));
}

/**
 * @brief Acepta una nueva conexión entrante en `serverSocket` y la registra.
 *
 * - Llama a `accept` para obtener un nuevo descriptor de cliente.
 * - Configura el socket como no bloqueante y con `FD_CLOEXEC`.
 * - Añade el descriptor al conjunto de `poll` con `POLLIN`.
 * - Crea un objeto `User` con nickname provisional `Guest<fd>` y lo añade
 *   al servidor mediante `addUser`.
 *
 * @throws errorAcceptingConnectionException Si `accept` falla.
 * @throws errorSettingNonblockingException Si `setSocketNonBlocking` falla.
 * @throws errorSettingCloexecException Si `setSocketCloexec` falla.
 *
 * @note El `User` se crea con `new` y su liberación queda a cargo de la
 *       gestión del `Server` (ej. `deleteUser` al desconectar).
 */
void Server::handleNewConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (newSocket < 0)
        throw errorAcceptingConnectionException(std::string("accept failed: ") + strerror(errno));

    setSocketNonBlocking(newSocket);
    setSocketCloexec(newSocket);
    pushPollFd(newSocket, POLLIN);

    std::stringstream ss;
    ss << "Guest" << newSocket;
    User* newUser = new User(newSocket, ss.str());
    addUser(newUser);

    std::cout << "New connection accepted | socket: " << newUser->getSocket() << " | nickname: " << newUser->getNickname() << std::endl;
}

/**
 * @brief Cierra y limpia la conexión asociada al índice `idx` en `fds`.
 *
 * Valida el índice (no permite cerrar el descriptor del servidor, normalmente
 * en `fds[0]`), cierra el descriptor, elimina el `User` asociado y borra la
 * entrada correspondiente en `fds` y en `buffers`.
 *
 * @param idx Índice en el vector `fds` del cliente a desconectar.
 *            `0` suele corresponder al `serverSocket` y no debe cerrarse aquí.
 * @note No lanza excepciones en condiciones normales; ante un `idx` inválido
 *       simplemente registra el error y retorna.
 */
void Server::handleDisconnection(int idx)
{
    int nfds = this->fds.size();
    if (idx <= 0 || idx >= nfds) {
        std::cerr << "handleDisconnection: invalid idx " << idx << std::endl;
        return;
    }

    int fd = this->fds[idx].fd;

    std::cout << "Client disconnected | fd: " << fd << std::endl;

    close(fd);
    this->deleteUser(fd);
    this->fds.erase(this->fds.begin() + idx);

    std::map<int, std::string>::iterator bit = this->buffers.find(fd);
    if (bit != this->buffers.end())
        this->buffers.erase(bit);
}
