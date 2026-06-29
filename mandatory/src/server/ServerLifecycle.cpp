/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLifecycle.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:22 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 17:01:12 by agarcia          ###   ########.fr       */
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
Server::Server(unsigned int port, std::string password): serverSocket(-1), port(port), password(password), services(this)
{
	char buf[256];
	if (gethostname(buf, sizeof(buf)) != 0)
		this->hostname = std::string("localhost");
	else
		this->hostname = std::string(buf);
}

/**
 * @brief Constructor de copia del servidor.
 *
 * Copia el puerto, la contraseña y el hostname del servidor `other`.
 * Inicializa `serverSocket` con -1 (no creado aún) y crea un nuevo
 * objeto `Services` asociado a este servidor.
 *
 * @param other Servidor a copiar.
 * @return Copia del servidor `other`.
 */
Server::Server(const Server &other) : serverSocket(-1), port(other.port), password(other.password), services(this)
{
	char buf[256];
	if (gethostname(buf, sizeof(buf)) != 0)
		this->hostname = std::string("localhost");
	else
		this->hostname = std::string(buf);
}

/**
 * @brief Operador de asignación del servidor.
 *
 * Asigna el puerto, la contraseña y el hostname del servidor `other` al actual.
 * Inicializa `serverSocket` con -1 (no creado aún) y crea un nuevo
 * objeto `Services` asociado a este servidor.
 *
 * @param other Servidor a asignar.
 * @return Referencia al servidor actual.
 */
Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->port = other.port;
		this->password = other.password;
		this->services = Services(this);
		this->serverSocket = -1;
		char buf[256];
		if (gethostname(buf, sizeof(buf)) != 0)
			this->hostname = std::string("localhost");
		else
			this->hostname = std::string(buf);
	}
	return *this;
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
/** 
 * @brief Crea el socket de escucha del servidor.
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
}

/** 
 * @brief Configura las opciones del socket de escucha.
 *
 * - Establece la opción `SO_REUSEADDR` para permitir reutilización de la dirección.
 * - Si falla, lanza una excepción con el mensaje de error.
 */
void Server::setSocketOptions()
{
	int opt = 1;
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("setsockopt failed: ") + strerror(errno));
}

/** 
 * @brief Vincula el socket de escucha del servidor a la dirección y puerto especificados.
 *
 * - Inicializa la estructura `sockaddr_in` con los valores adecuados.
 * - Si falla, lanza una excepción con el mensaje de error.
 */
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

/** 
 * @brief Pone el socket de escucha del servidor en modo de escucha.
 *
 * - Usa `listen()` para poner el socket en modo de escucha.
 * - Si falla, lanza una excepción con el mensaje de error.
 */
void Server::listenServerSocket()
{
	if (listen(this->serverSocket, 64) < 0)
		throw IrcException(IRC_ERR_STARTING_SERVER, std::string("listen failed: ") + strerror(errno));
}

/** 
 * @brief Configura los descriptores de archivo para el polling.
 *
 * - Limpia la lista de descriptores de archivo.
 * - Añade el socket del servidor a la lista.
 */
void Server::setupPollFds()
{
	this->fds.clear();
	pushPollFd(this->serverSocket, POLLIN);
}

/** 
 * @brief Añade un descriptor de archivo a la lista para el polling.
 *
 * - Crea una estructura `pollfd` con los valores dados.
 * - Añade la estructura a la lista `fds`.
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
 * @brief Inicia el servidor.
 *
 * - Establece la bandera `running` a true.
 * - Establece la instancia del servidor.
 * - Crea el socket del servidor.
 */
void Server::startServer()
{
	running = true;
	instance = this;
	createServerSocket();
	setSocketOptions();
	bindServerSocket();
	listenServerSocket();
	setupPollFds();
	initSignals();
	handleEvents();
}

/** 
 * @brief Detiene el servidor.
 *
 * - Establece la bandera `running` a false.
 * - Cierra el socket del servidor si está abierto.
 */
void Server::stopServer()
{
	running = false;
	if (this->serverSocket >= 0) {
		close(this->serverSocket);
		this->serverSocket = -1;
	}
}

/** 
 * @brief Limpia los recursos del servidor.
 *
 * - Cierra todos los descriptores de archivo.
 * - Limpia las listas de usuarios y canales.
 * - Establece el socket del servidor a -1.
 */
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


/** 
 * @brief Obtiene la instancia actual del servidor.
 *
 * - Devuelve un puntero al objeto `Server` actualmente en ejecución.
 * - Si no hay instancia, devuelve NULL.
 */
Server* Server::getInstance()
{
	return Server::instance;
}
