/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_lifecycle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:29 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 18:08:19 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "socket_utils.h"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @brief Constructor del servidor.
 *
 * Inicializa el puerto y la contraseña del servidor, y prepara el
 * descriptor `serverSocket` con valor -1 (no creado aún).
 *
 * @param port Puerto en el que el servidor escuchará conexiones.
 * @param password Contraseña requerida para autenticación (puede estar vacía).
 */
Server::Server(unsigned int port, std::string password): port(port), password(password)
{
    this->serverSocket = -1;
    std::cout << "Server created" << std::endl;
}

/**
 * @brief Destructor del servidor: cierra sockets y libera recursos.
 *
 * - Cierra todos los sockets cliente almacenados en `fds` (a partir de 1).
 * - Cierra `serverSocket` si está abierto.
 * - Libera memoria de los objetos `User` y `Channel` restantes.
 */
Server::~Server()
{
    for (size_t i = 1; i < this->fds.size(); ++i)
    {
        if (this->fds[i].fd >= 0)
            close(this->fds[i].fd);
    }
    if (this->serverSocket >= 0)
        close(this->serverSocket);
    // delete remaining users
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it) {
        delete it->second;
    }
    this->users.clear();
    // delete remaining channels
    for (std::map<std::string, Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        delete it->second;
    }
    this->channels.clear();
    std::cout << "Server destroyed" << std::endl;
}
/**
 * @brief Crea el socket de escucha del servidor.
 *
 * Crea un socket TCP IPv4, y configura `O_NONBLOCK` y `FD_CLOEXEC`.
 * Lanza `errorStartingServerException` si la creación falla.
 */
void Server::createServerSocket()
{
    // Create socket
    // AF_INET: IPv4, SOCK_STREAM: TCP
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0)
        throw errorStartingServerException(0, std::string("socket failed: ") + strerror(errno));
    setSocketNonBlocking(this->serverSocket);
    setSocketCloexec(this->serverSocket);
}
/**
 * @brief Configura opciones del socket de servidor (ej. `SO_REUSEADDR`).
 *
 * Lanza `errorStartingServerException` si `setsockopt` falla.
 */
void Server::setSocketOptions()
{
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw errorStartingServerException(0, std::string("setsockopt failed: ") + strerror(errno));
    std::cout << "Socket options set" << std::endl;
}

/**
 * @brief Enlaza (`bind`) el `serverSocket` al puerto configurado.
 *
 * Lanza `errorStartingServerException` si `bind` falla.
 */
void Server::bindServerSocket()
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw errorStartingServerException(0, std::string("bind failed: ") + strerror(errno));
    std::cout << "Bind successful" << std::endl;
}

/**
 * @brief Pone el `serverSocket` en modo escucha (`listen`).
 *
 * @throws errorStartingServerException si `listen` falla.
 */
void Server::listenServerSocket()
{
    if (listen(this->serverSocket, 64) < 0)
        throw errorStartingServerException(0, std::string("listen failed: ") + strerror(errno));
    std::cout << "Server is listening on port " << this->port << "..." << std::endl;
}

/**
 * @brief Inicializa la lista `fds` usada por `poll` y registra el socket
 * de servidor para eventos de lectura.
 */
void Server::setupPollFds()
{
    this->fds.clear();
    pushPollFd(this->serverSocket, POLLIN);
}

/**
 * @brief Añade un `pollfd` al vector `fds` con los `events` indicados.
 *
 * @param fd Descriptor de archivo a monitorizar.
 * @param events Máscara de eventos (ej. `POLLIN`).
 */
void Server::pushPollFd(int fd, short events)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    this->fds.push_back(pfd);
}

/**
 * @brief Inicializa y arranca el ciclo principal del servidor.
 *
 * - Crea y configura el socket, lo enlaza y pone en escucha.
 * - Prepara `fds` y luego entra en `handleEvents()` que gestiona el bucle
 *   principal de `poll`.
 *
 * Si ocurre un error en la fase de inicio, cierra el socket y muestra el
 * error por `stderr`.
 */
void Server::startServer()
{
    try {
        createServerSocket();
        setSocketOptions();
        bindServerSocket();
        listenServerSocket();
        setupPollFds();
    }
    catch (const std::exception &e)
    {
        if(this->serverSocket >= 0) {
            close(this->serverSocket);
            this->serverSocket = -1;
        }
        std::cerr << "ERROR: " << e.what() << std::endl;
        return;
    }
    handleEvents();
}

/**
 * @brief Detiene el servidor cerrando el socket de escucha.
 */
void Server::stopServer()
{
    if (this->serverSocket >= 0) {
        close(this->serverSocket);
        this->serverSocket = -1;
    }
    std::cout << "Server stopped" << std::endl;
}
