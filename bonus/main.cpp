/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:51 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:52 by adriescr         ###   ########.fr       */
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

int	chekPort(const char* portStr, unsigned int &port) {
	char *endptr = NULL;
	long p = std::strtol(portStr, &endptr, 10);
	if (*endptr != '\0' || p <= 0 || p > 65535) {
		std::cerr << "Invalid port number. Please provide a port number between 1 and 65535." << std::endl;
		return 1;
	}
	port = static_cast<unsigned int>(p);
	return 0;
}

int	checkArgs(int argc, char* argv[], unsigned int &port, std::string &password) {
	if (argc == 2) {
		// Forma: ./ircserv <password>  -> usar puerto por defecto
		port = DEFAULT_PORT;
		password = argv[1];
	} else if (argc == 3) {
		// Forma: ./ircserv <port> <password>
		if (chekPort(argv[1], port) != 0) {
			return 1;
		}
		password = argv[2];
	} else {
		std::cerr << "Usage: " << argv[0] << " [port] <password>" << std::endl;
		return 1;
	}
	return 0;
}

/**
 * @brief Punto de entrada de la aplicación.
 *
 * Valida argumentos (acepta `./ircserv [port] <password>`), crea el `Server`
 * y arranca el loop principal. Captura excepciones fatales y muestra un mensaje
 * antes de terminar.
 */
int main(int argc, char* argv[]) {
	unsigned int port;
	std::string password;

	if (checkArgs(argc, argv, port, password) != 0)
		return 1;

	try {
		Server server(port, password);
		server.startServer();
	} catch (const std::exception &e) {
		std::cerr << "Fatal: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Fatal: Unknown exception" << std::endl;
		return 1;
	}
	std::cout << "Server stopped gracefully." << std::endl;
	return 0;
}
