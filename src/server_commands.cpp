/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:23 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/20 17:58:34 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <utility>

/**
 * @brief Parsea una línea de comando enviada por un cliente y la ejecuta.
 *
 * Extrae el nombre del comando (texto hasta el primer espacio) y los
 * parámetros restantes. Registra el comando y delega la ejecución a
 * `executeCommand`.
 *
 * @param user Puntero al `User` que envió la línea.
 * @param commandLine Línea completa recibida del cliente (puede incluir CR/LF).
 */
void Server::handleClientCommand(User *user, const std::string &commandLine)
{
    if (!user) {
        std::cerr << "handleClientCommand: null user for command: " << commandLine << std::endl;
        return;
    }
    size_t sp = commandLine.find(' ');
    std::string cmd;
    std::string args;
    if (sp == std::string::npos) {
        cmd = commandLine;
        args = "";
    } else {
        cmd = commandLine.substr(0, sp);
        if (sp + 1 < commandLine.size())
            args = commandLine.substr(sp + 1);
        else
            args = "";
    }
    std::cout << "[" << cmd << "] " << "fd:" << user->getSocket() << " args:" << (args.empty() ? "<EMPTY>" : args) << std::endl;
    executeCommand(user, cmd, args);
}

/**
 * @brief Enruta y ejecuta el comando recibido según el estado de autenticación.
 *
 * - Si el `user` no está autenticado permite solo `PASS`, `NICK` y `USER`.
 *   Si tras esos valores el usuario queda completo, marca `authenticated`.
 * - Si está autenticado despacha los comandos conocidos a sus handlers
 *   correspondientes. En caso de comando no reconocido llama a
 *   `handleUnknownCommand`.
 *
 * @param user Usuario emisor del comando.
 * @param command Nombre del comando (esperado en mayúsculas).
 * @param args Parámetros asociados al comando.
 */
void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{
    if (!user->isAuthenticated())
    {
        if (user->isPassSet() && user->isNickSet() && user->isUserSet())
        {
            user->setAuthenticated(true);
            sendToUser(user, std::string("Authentication successful"));
            return;
        }

        if (command == "PASS") {handlePASS(user, args); return;}
        if (command == "NICK") {handleNICK(user, args); return;}
        if (command == "USER") {handleUSER(user, args); return;}
    
        sendToUser(user, std::string("You're not authenticated"));
        return;
    }

    if(command == "JOIN") { handleJOIN(user, args); return;}
    if(command == "PART") { handlePART(user, args); return;}
    if(command == "PRIVMSG") { handlePRIVMSG(user, args); return;}
    if(command == "QUIT") { handleQUIT(user, args); return;}
    if(command == "LIST") { handleLIST(user, args); return;}
    if(command == "INVITE") { handleINVITE(user, args); return;}
    if(command == "KICK") { handleKICK(user, args); return;}
    if(command == "MODE") { handleMODE(user, args); return;}

    handleUnknownCommand(user, command);
}
