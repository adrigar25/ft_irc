/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:39:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/11 16:20:06 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketUtils.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <cstring>
#include <errno.h>

/**
 * @brief Establece un socket como no bloqueante.
 *
 * Usa `fcntl` para obtener las flags actuales del descriptor `fd`, y luego
 * añade la flag `O_NONBLOCK`. Si ocurre un error, lanza una excepción con
 * el mensaje de error correspondiente.
 *
 * @param fd Descriptor del socket a modificar.
 * @throws std::runtime_error Si `fcntl` falla al obtener o establecer las flags.
*/
void setSocketNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error(std::string("fcntl F_GETFL: ") + strerror(errno));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error(std::string("fcntl F_SETFL: ") + strerror(errno));
    }
}

/**
 * @brief Establece la flag FD_CLOEXEC en un descriptor de archivo.
 *
 * Esto asegura que el descriptor se cierre automáticamente si el proceso ejecuta
 * otro programa con `exec`. Usa `fcntl` para modificar las flags del descriptor.
 * Si ocurre un error, lanza una excepción con el mensaje de error correspondiente.
 *
 * @param fd Descriptor del socket a modificar.
 * @throws std::runtime_error Si `fcntl` falla al obtener o establecer las flags.
 */
void setSocketCloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        throw std::runtime_error(std::string("fcntl F_GETFD: ") + strerror(errno));
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
        throw std::runtime_error(std::string("fcntl F_SETFD: ") + strerror(errno));
}

/**
 * @brief Formatea un mensaje para asegurarse de que termina con CRLF.
 *
 * En el protocolo IRC, los mensajes deben terminar con "\r\n". Esta función
 * verifica si el mensaje ya contiene esta terminación, y si no, la añade.
 *
 * @param message El mensaje a formatear.
 * @return El mensaje formateado con CRLF al final.
 */

std::string formatMessage(const std::string &message)
{
    std::string msg = message;
    if (msg.find("\r\n") == std::string::npos)
        msg += "\r\n";
    return msg;
}
