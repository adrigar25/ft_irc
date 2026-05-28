#include "Server.hpp"
#include "socket_utils.h"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


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
        throw IrcException(IRC_ERR_ACCEPTING_CONNECTION, std::string("accept failed: ") + strerror(errno));

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
void Server::handleDisconnectionByIndex(int idx)
{
    if (idx <= 0 || idx >= (int)this->fds.size()) {
        std::cerr << "handleDisconnectionByIndex: invalid idx " << idx << std::endl;
        return;
    }

    int fd = this->fds[idx].fd;

    std::cout << "Client disconnected | fd: " << fd << std::endl;

    close(fd);
    this->deleteUser(fd);
    this->fds.erase(this->fds.begin() + idx);
}

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
