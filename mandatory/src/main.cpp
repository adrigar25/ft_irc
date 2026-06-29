/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:51 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/28 16:43:08 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include "Server.hpp"
#include <string>

/**
 * @brief Valida el puerto proporcionado como argumento.
 *
 * @param portStr Cadena que representa el puerto.
 * @param port Referencia a la variable donde se almacenará el puerto validado.
 * @return int 0 si es válido, 1 si no lo es.
 */
int checkPort(const char *portStr, unsigned int &port)
{
	char *endptr = NULL;
	long p = std::strtol(portStr, &endptr, 10);
	if (*endptr != '\0' || p <= 0 || p > 65535)
	{
		std::cerr << "Invalid port number. Please provide a port number between 1 and 65535.\n(PORT set to default " << DEFAULT_PORT << ")" << std::endl;
		return 1;
	}
	port = static_cast<unsigned int>(p);
	return 0;
}

/**
 * @brief Valida los argumentos de línea de comandos.
 *
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @param port Referencia a la variable donde se almacenará el puerto validado.
 * @param password Referencia a la variable donde se almacenará la contraseña.
 * @return int 0 si los argumentos son válidos, 1 si no lo son.
 */
int checkArgs(int argc, char *argv[], unsigned int &port, std::string &password)
{

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}
	if (checkPort(argv[1], port) != 0)
		port = DEFAULT_PORT;
	password = argv[2];

	return 0;
}

/**
 * @brief Punto de entrada de la aplicación.
 *
 * Valida argumentos (acepta `./ircserv [port] <password>`), crea el `Server`
 * y arranca el loop principal. Captura excepciones fatales y muestra un mensaje
 * antes de terminar.
 */
int main(int argc, char *argv[])
{
	unsigned int port;
	std::string password;

	if (checkArgs(argc, argv, port, password) != 0)
		return 1;

	try
	{
		Server server(port, password);
		server.startServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Fatal: " << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Fatal: Unknown exception" << std::endl;
		return 1;
	}
	std::cout << "Server stopped gracefully." << std::endl;
	return 0;
}
