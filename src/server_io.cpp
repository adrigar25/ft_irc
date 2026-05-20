/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_io.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:27 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 18:08:19 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "socket_utils.h"
#include <iostream>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>

/*
 * @brief Habilita la vigilancia de POLLOUT para un fd concreto en el vector
 * `this->fds`. Extraído para evitar código duplicado.
 */
void Server::enablePollOutForFd(int fd)
{
    for (size_t i = 1; i < this->fds.size(); ++i)
    {
        if (this->fds[i].fd == fd)
        {
            this->fds[i].events |= POLLOUT;
            break;
        }
    }
}

// formatMessage moved to socket_utils.cpp


void Server::enqueuePending(User *user, const char *buf, size_t len)
{
    std::string &pending = user->getOutBuffer();
    pending.append(buf, len);
    enablePollOutForFd(user->getSocket());
}

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
    std::string msg = formatMessage(message);
    int fd = user->getSocket();

    std::string &pending = user->getOutBuffer();
    size_t &offset = user->getOutOffset();

    // If there's already pending data, enqueue and ensure POLLOUT
    if (!pending.empty() || offset != 0)
    {
        enqueuePending(user, msg.c_str(), msg.size());
        return;
    }

    const char *buf = msg.c_str();
    size_t total = 0;
    size_t len = msg.size();
    ssize_t n;

    while (total < len)
    {
        n = send(fd, buf + total, len - total, 0);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                enqueuePending(user, buf + total, len - total);
                break;
            }
            std::cerr << "send failed: " << strerror(errno) << std::endl;
            break;
        }
        total += n;
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
 * @param exclude Puntero al usuario a excluir de la lista de destinatarios.
 */
void Server::sendToChannel(Channel *channel, const std::string &message, User *exclude)
{
    const std::map<int, User*>& usersMap = channel->getUsers();
    for (std::map<int, User*>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
    {
        if (it->second && it->second != exclude)
            sendToUser(it->second, message);
    }
}
