/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:17 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:53:28 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

/**
 * @brief Convierte una `exception_ptr` en un mensaje legible.
 *
 * Intenta re-lanzar la excepción y capturar diversos tipos para obtener
 * un mensaje descriptivo.
 * @param ep `std::exception_ptr` apuntando a la excepción.
 * @return Cadena con el mensaje de la excepción.
 */
static std::string exception_message(std::exception_ptr ep) {
        try {
                if (ep) std::rethrow_exception(ep);
        } catch (const std::exception &e) { return e.what(); }
            catch (const std::string &s) { return s; }
            catch (const char *s) { return std::string(s); }
            catch (...) { return "Unknown exception"; }
        return {};
}


/**
 * @brief Valida los argumentos de línea de comandos esperados.
 *
 * Comprueba que se pasen exactamente 2 argumentos (puerto y contraseña)
 * y que el puerto esté en el rango válido.
 * @return `true` si los argumentos son válidos.
 */
static bool checkArgs(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" <<std::endl;
        return false;
    }
    if(std::atoi(argv[1]) <= 0 || std::atoi(argv[1]) > 65535) {
        std::cerr << "Invalid port number. Please provide a port number between 1 and 65535." << std::endl;
        return false;
    }
    return true;
}


/**
 * @brief Punto de entrada de la aplicación.
 *
 * Valida argumentos, crea el `Server` y arranca el loop principal. Captura
 * excepciones fatales y muestra un mensaje antes de terminar.
 */
int main(int argc, char* argv[]) {
    
    if (!checkArgs(argc, argv))
        return 1;
    try {
        Server server(std::atoi(argv[1]), argv[2]);
        server.startServer();
    } catch (...) {
        std::cerr << "Fatal: " << exception_message(std::current_exception()) << std::endl;
        return 1;
    }
    return 0;
}