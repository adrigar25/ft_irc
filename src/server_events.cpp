/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_events.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:25 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/17 00:55:51 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>


/**
 * @brief Envuelve la llamada a `poll` sobre el vector `fds` del servidor.
 *
 * @return Número de descriptores listos (>0), 0 si la llamada fue
 * interrumpida por una señal (`EINTR`), o -1 en caso de error.
 */
int Server::performPoll()
{
    int nfds = this->fds.size();
    int ready = poll(this->fds.data(), nfds, -1);
    if (ready < 0) {
        if (errno == EINTR) return 0;
        std::cerr << "poll error: " << strerror(errno) << std::endl;
        return -1;
    }
    return ready;
}


/**
 * @brief Lee datos disponibles del cliente en `fds[idx]` y los acumula.
 *
 * - Verifica que `idx` sea válido y que el descriptor tenga `POLLIN`.
 * - Llama a `recv` y, si `n <= 0`, considera que el cliente se ha
 *   desconectado y llama a `handleDisconnection(idx)` retornando `true`.
 * - Si hay datos, los añade al buffer asociado (`this->buffers[fd]`) y
 *   llama a `processClientBuffer(fd)` para procesar líneas completas.
 *
 * @param idx Índice en `this->fds` del descriptor a leer.
 * @return `true` si se produjo una desconexión (caller debe reajustar
 *         su índice), `false` en caso contrario.
 */
bool Server::handleClientRead(int idx)

{
    int fd = -1;
    char buffer[1024];

    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (!(this->fds[idx].revents & POLLIN))
        return false;
    fd = this->fds[idx].fd;
    ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (n <= 0)
    {
        handleDisconnection(idx);
        return true;
    }
    buffer[n] = '\0';
    std::string &acc = this->buffers[fd];
    acc.append(buffer, n);
    processClientBuffer(fd);
    return false;
}


/**
 * @brief Procesa el buffer acumulado de un cliente, línea por línea.
 *
 * - Extrae líneas separadas por '\n', elimina '\r' final si existe y
 *   llama a `handleClientCommand` para cada línea.
 * - Captura excepciones lanzadas por el procesamiento del comando y,
 *   en caso de error, desconecta al cliente.
 *
 * @param fd Descriptor de archivo (socket) del cliente cuyo buffer se procesa.
 */
void Server::processClientBuffer(int fd)
{
    std::string &buffer = this->buffers[fd];
    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos);
        if (!line.empty() && line.back() == '\r')
            line.erase(line.size() - 1);
        try {
            handleClientCommand(this->users[fd], line);
        } catch (const std::exception &e) {
            std::cerr << "Client error fd " << fd << ": " << e.what() << std::endl;
            for (int i = 1; i < (int)this->fds.size(); ++i) {
                if (this->fds[i].fd == fd) { handleDisconnection(i); break; }
            }
        } catch (...) {
            std::cerr << "Unknown client error fd " << fd << std::endl;
            for (int i = 1; i < (int)this->fds.size(); ++i) {
                if (this->fds[i].fd == fd) { handleDisconnection(i); break; }
            }
        }
        buffer.erase(0, pos + 1);
    }
}


/**
 * @brief Maneja eventos de error/hangup en el descriptor `fds[idx]`.
 *
 * Si se detecta `POLLHUP`, `POLLERR` o `POLLNVAL` desconecta al cliente.
 *
 * @param idx Índice en `this->fds` a comprobar.
 * @return `true` si se realizó una desconexión, `false` en caso contrario.
 */
bool Server::handleClientErrorEvents(int idx)
{
    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (this->fds[idx].revents & (POLLHUP | POLLERR | POLLNVAL))
    {
        handleDisconnection(idx);
        return true;
    }
    return false;
}


/**
 * @brief Recorre los descriptores de cliente y procesa lectura y errores.
 *
 * Itera sobre `this->fds` (desde 1 para saltarse el `serverSocket`) y
 * llama a `handleClientRead` y `handleClientErrorEvents`. Si alguno de los
 * handlers devuelve `true` (desconexión), decrementa el índice para mantener
 * la coherencia del bucle tras la eliminación del descriptor.
 */
void Server::handleClientEvents()
{
    for (int i = 1; i < (int)this->fds.size(); ++i)
    {
        if (handleClientWrite(i)) { i--; continue; }
        if (handleClientRead(i)) { i--; continue; }
        if (handleClientErrorEvents(i)) { i--; continue; }
    }
}


bool Server::handleClientWrite(int idx)
{
    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (!(this->fds[idx].revents & POLLOUT))
        return false;
    int fd = this->fds[idx].fd;
    std::map<int, std::string>::iterator it = this->outBuffers.find(fd);
    if (it == this->outBuffers.end() || it->second.empty()) {
        setPollOut(fd, false);
        return false;
    }
    std::string &buf = it->second;
    while (!buf.empty()) {
        ssize_t n = send(fd, buf.c_str(), buf.size(), 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) { setPollOut(fd, true); break; }
            std::cerr << "send failed on fd " << fd << ": " << strerror(errno) << std::endl;
            handleDisconnection(idx);
            return true;
        }
        buf.erase(0, static_cast<size_t>(n));
    }
    if (buf.empty()) setPollOut(fd, false);
    return false;
}


void Server::setPollOut(int fd, bool enable)
{
    for (size_t i = 0; i < this->fds.size(); ++i) {
        if (this->fds[i].fd == fd) {
            if (enable) this->fds[i].events |= POLLOUT;
            else this->fds[i].events &= ~POLLOUT;
            break;
        }
    }
}


/*
 * @brief Maneja los eventos del servidor, incluyendo conexiones nuevas y eventos de clientes.
 * - En el bucle principal, llama a `performPoll` para esperar eventos.
 * - Si `performPoll` devuelve >0, verifica si el primer descriptor (`serverSocket`) tiene `POLLIN` para aceptar nuevas conexiones.
 * - Luego llama a `handleClientEvents` para procesar eventos de lectura y errores en los clientes conectados.
 * - Captura y registra cualquier excepción que ocurra durante la aceptación de conexiones o el procesamiento de eventos de clientes, sin abortar el
 */
void Server::handleEvents()
{
    int ready = 0;
    while (true)
    {
        if (this->fds.size() == 0)
            continue;
        ready = performPoll();
        if (ready == 0)
            continue;
        if (ready < 0)
            break;
        if (!this->fds.empty() && (this->fds[0].revents & POLLIN))
        {
            try {
                handleNewConnection();
            } catch (const std::exception &e) {
                std::cerr << "Accept error: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown accept error" << std::endl;
            }
        }

        try {
            handleClientEvents();
        } catch (const std::exception &e) {
            std::cerr << "Error processing client events: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error processing client events" << std::endl;
        }
    }
}
