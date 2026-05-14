/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_io.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:27 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:22:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>

/**
 * @brief Envía un mensaje completo a un `User`, manejando envíos parciales.
 *
 * Asegura que el `message` termine en CRLF (`\r\n`) si no lo tiene,
 * y usa `send()` en un bucle para completar la escritura cuando `send`
 * devuelve menos bytes de los esperados (envíos parciales). Maneja errores
 * comunes: reintenta en `EINTR`, detecta `EAGAIN`/`EWOULDBLOCK` (socket en
 * modo no bloqueante) y registra fallos en `send`.
 *
 * @param user Puntero al `User` destino (se usa `user->getSocket()`).
 * @param message Texto a enviar; puede no incluir CRLF.
 *
 * @note Si `send` devuelve `EAGAIN`/`EWOULDBLOCK`, la función sale sin
 *       reintentar continuamente; los bytes no enviados no se encolan aquí.
 */
void Server::sendToUser(User *user, const std::string &message)
{
    std::string msg = message;
    if (msg.find("\r\n") == std::string::npos)
        msg += "\r\n";

    const char *buf = msg.c_str();
    size_t total = 0;
    size_t len = msg.size();

    while (total < len)
    {
        ssize_t n = send(user->getSocket(), buf + total, len - total, 0);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "send would block on fd " << user->getSocket() << std::endl;
                break;
            }
            std::cerr << "send failed: " << strerror(errno) << std::endl;
            break;
        }
        total += static_cast<size_t>(n);
    }
}

/**
 * @brief Envía `message` a todos los usuarios registrados en `channel`.
 *
 * Itera sobre el mapa de usuarios del canal y llama a `sendToUser` por
 * cada `User*`. No realiza filtrado adicional (por ejemplo, no excluye al
 * emisor); cualquier lógica de exclusión debe aplicarse antes de llamar
 * a esta función.
 *
 * @param channel Canal destino cuyos usuarios recibirán el mensaje.
 * @param message Mensaje a enviar a cada usuario.
 */
void Server::sendToChannel(Channel *channel, const std::string &message)
{
    const std::map<int, User*>& usersMap = channel->getUsers();
    for (std::map<int, User*>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
        sendToUser(it->second, message);
}
