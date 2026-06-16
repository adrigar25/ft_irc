/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLifecycle.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:22 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:00:24 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "SocketUtils.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fstream>
#include <limits.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <exception>


Server* Server::instance = NULL;

/**
 * @brief Constructor del servidor.
 *
 * Inicializa el puerto y la contraseña del servidor, y prepara el
 * descriptor `serverSocket` con valor -1 (no creado aún).
 *
 * @param port Puerto en el que el servidor escuchará conexiones.
 * @param password Contraseña requerida para autenticación (puede estar vacía).
 */
Server::Server(unsigned int port, std::string password): port(port), password(password), services(this)
{
	this->serverSocket = -1;
	char buf[256];
	if (gethostname(buf, sizeof(buf)) != 0)
		this->hostname = std::string("localhost");
	else
		this->hostname = std::string(buf);
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
	this->services.users().clear();
	this->services.channels().clear();
}
/** @brief Crea el socket de escucha del servidor.
 * - Usa `socket(AF_INET, SOCK_STREAM, 0)` para crear un socket TCP.
 * - Si falla, lanza una excepción con el mensaje de error.
 * - Configura el socket como no bloqueante y con la bandera CLOEXEC.
 */
void Server::createServerSocket()
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("socket failed: ") + strerror(errno));
	setSocketNonBlocking(this->serverSocket);
	setSocketCloexec(this->serverSocket);
}

void Server::setSocketOptions()
{
	int opt = 1;
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("setsockopt failed: ") + strerror(errno));
}

void Server::bindServerSocket()
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->port);

	if (bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("bind failed: ") + strerror(errno));
}

void Server::listenServerSocket()
{
	if (listen(this->serverSocket, 64) < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("listen failed: ") + strerror(errno));
}

void Server::setupPollFds()
{
	this->fds.clear();
	pushPollFd(this->serverSocket, POLLIN);
}

void Server::pushPollFd(int fd, short events)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	this->fds.push_back(pfd);
}

void Server::startServer()
{
	try {
		running = true;
		instance = this;
		createServerSocket();
		setSocketOptions();
		bindServerSocket();
		listenServerSocket();
		setupPollFds();
		initSignals();
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

void Server::stopServer()
{
	running = false;
	if (this->serverSocket >= 0) {
		close(this->serverSocket);
		this->serverSocket = -1;
	}
}

void Server::cleanup()
{
	for (size_t i = 0; i < this->fds.size(); ++i)
	{
		if (this->fds[i].fd >= 0)
			close(this->fds[i].fd);
	}

	this->fds.clear();
	this->services.users().clear();
	this->services.channels().clear();
	this->serverSocket = -1;
}



Server* Server::getInstance()
{
	return Server::instance;
}
