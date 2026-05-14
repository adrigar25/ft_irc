/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:23 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:22:48 by agarcia          ###   ########.fr       */
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
    std::string cmd = commandLine.substr(0, commandLine.find(' '));
    std::string args = "";
    if(commandLine[cmd.size() + 1])
        args = commandLine.substr(cmd.size() + 1);
    std::cout << "[" << cmd << "] " << "fd:" << user->getSocket() << " args:" << (args.empty() ? "<EMPTY>" : args) << std::endl;
    executeCommand(user, cmd, args);
}


/**
 * @brief Maneja el comando PASS para establecer la contraseña del usuario.
 *
 * Si `params` está vacío envía `461`. Si la contraseña coincide con
 * `this->password` marca la contraseña del `User` como establecida y
 * confirma; en caso contrario notifica `Invalid password`.
 *
 * @param user Usuario que envía la contraseña.
 * @param params Parámetros del comando (la contraseña esperada).
 * @return Siempre devuelve true (protocol handler).
 */
bool Server::handlePASS(User *user, const std::string &params)
{
    if (params.empty()) {
        sendToUser(user, std::string("461 PASS :Not enough parameters"));
        return true;
    }
    if (params == this->password) {
        user->setPass(true);
        sendToUser(user, std::string("Password accepted"));
        return true;
    }
    sendToUser(user, std::string("Invalid password"));
    return true;
}

/**
 * @brief Maneja el comando NICK para asignar el apodo del usuario.
 *
 * Verifica que `nick` no esté vacío y que no esté ya en uso por otro
 * usuario. Si todo es correcto establece el nickname en `user`.
 *
 * @param user Usuario que solicita el cambio de nickname.
 * @param nick Nuevo nickname solicitado.
 * @return Siempre devuelve true.
 */
bool Server::handleNICK(User *user, const std::string &nick)
{
    if (nick.empty()) {
        sendToUser(user, std::string("431 NICK :No nickname given"));
        return true;
    }
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it)
    {
        if (it->second != user && it->second->getNickname() == nick)
        {
            sendToUser(user, std::string("433 NICK :Nickname is already in use"));
            return true;
        }
    }
    user->setNickname(nick);
    sendToUser(user, std::string("Nickname set"));
    return true;
}

/**
 * @brief Maneja el comando USER para establecer información del usuario.
 *
 * Parsea los parámetros en `username`, `mode`, `unused` y `real` (texto
 * después del tercer espacio). Elimina el prefijo `:` de la parte real si
 * está presente. Marca la información del usuario y, si ya están presentes
 * PASS y NICK, intenta autenticar al usuario.
 *
 * @param user Usuario que envía el comando.
 * @param params Parámetros del comando USER.
 * @return Siempre devuelve true.
 */
bool Server::handleUSER(User *user, const std::string &params)
{
    if (params.empty()) {
        sendToUser(user, std::string("461 USER :Not enough parameters"));
        return true;
    }
    std::istringstream iss(params);
    std::string username, mode, unused, real;
    iss >> username >> mode >> unused;
    std::getline(iss, real);
    if (!real.empty() && real[0] == ' ') real.erase(0,1);
    if (!real.empty() && real[0] == ':') real.erase(0,1);

    user->setUsername(username);
    sendToUser(user, std::string("Username set"));

    if (user->isPassSet() && user->isNickSet() && user->isUserSet()) {
        user->setAuthenticated(true);
        sendToUser(user, std::string("Authentication successful"));
    }
    return true;
}

/**
 * @brief Maneja el comando QUIT; envía un mensaje de despedida.
 *
 * No realiza el cierre del socket aquí: el cliente normalmente lo cierra
 * tras recibir el mensaje. `params` no se utiliza actualmente.
 *
 * @param user Usuario que envía QUIT.
 * @param params Parámetros opcionales del comando (ignorados).
 * @return Siempre devuelve true.
 */
bool Server::handleQUIT(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("Goodbye!"));
    // The client will close the connection after receiving this message, so we don't need to do anything else here
    return true;
}

/**
 * @brief Envía al usuario la lista de canales y su número de usuarios.
 *
 * `params` se ignora actualmente. Construye una cadena con cada canal y
 * su tamaño y la envía al usuario.
 *
 * @param user Usuario solicitante.
 * @param params Parámetros del comando (no usados).
 * @return Siempre devuelve true.
 */
bool Server::handleLIST(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    std::string response = "Channels:\n";
    for (std::map<std::string, Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        response += it->first + " (" + std::to_string(it->second->getUserCount()) + " users)\n";
    sendToUser(user, response);
    return true;
}

/**
 * @brief Handler placeholder para INVITE (no implementado).
 *
 * Envía un mensaje indicándolo. Implementar la lógica de invitación
 * requerirá verificación de permisos y la gestión del estado del canal.
 */
bool Server::handleINVITE(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("INVITE command not implemented yet"));
    return true;
}

/**
 * @brief Handler placeholder para KICK (no implementado).
 */
bool Server::handleKICK(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("KICK command not implemented yet"));
    return true;
}

/**
 * @brief Handler placeholder para MODE (no implementado).
 */
bool Server::handleMODE(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("MODE command not implemented yet"));
    return true;
}

/* createChannel moved to src/server_channels.cpp */

/**
 * @brief Maneja el comando JOIN para unir a un usuario a uno o varios canales.
 *
 * - Separa `params` en la lista de canales (coma-separada) y opciones/keys
 *   (si existe un espacio separador).
 * - Valida cada nombre de canal (debe comenzar por '#', '&', '+', o '!'),
 *   limpia espacios y construye una lista de `channelNames`.
 * - Si se suministran keys se almacenan en `keys` (actualmente no usadas
 *   en la lógica de unión aquí, pero quedan parseadas).
 * - Para cada `channelName` existente llama a `user->joinChannel(...)`, y
 *   si no existe crea el canal con `createChannel(channelName, user)`.
 *
 * @param user Usuario que solicita unirse a los canales.
 * @param params Lista de canales (coma-separada) y opcionalmente keys.
 * @return Siempre devuelve true.
 */
bool Server::handleJOIN(User *user, const std::string &params)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = params;
    std::string keysPart;
    size_t sp = params.find(' ');
    if (sp != std::string::npos) {
        channelsPart = params.substr(0, sp);
        keysPart = params.substr(sp + 1);
    }

    std::istringstream ciss(channelsPart);
    std::string value;
    while (std::getline(ciss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            channelNames.push_back(value);
        else if (!value.empty())
            sendToUser(user, std::string("Invalid channel name: ") + value);
    }

    if (!keysPart.empty()) {
        std::istringstream kiss(keysPart);
        while (std::getline(kiss, value, ',')) {
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);
            if (!value.empty())
                keys.push_back(value);
        }
    }

    for (const auto &channelName : channelNames)
    {
        if(this->channels.find(channelName) == this->channels.end())
            createChannel(channelName, user);
        else
            user->joinChannel(this->channels[channelName]);
    }
    return true;
}

/**
 * @brief Maneja el comando PART para abandonar canales.
 *
 * Parsea una lista coma-separada de canales en `params` y para cada uno
 * (si existe) llama a `user->leaveChannel(...)`. `msgPart` puede contener
 * un motivo pero actualmente no se envía ni registra.
 *
 * @param user Usuario que solicita abandonar canales.
 * @param params Lista de canales (coma-separada) y opcionalmente un mensaje.
 * @return Siempre devuelve true.
 */
bool Server::handlePART(User *user, const std::string &params)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = params;
    std::string msgPart;
    size_t sp = params.find(' ');
    if (sp != std::string::npos) {
        channelsPart = params.substr(0, sp);
        msgPart = params.substr(sp + 1);
    }

    std::istringstream ciss(channelsPart);
    std::string value;
    while (std::getline(ciss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            channelNames.push_back(value);
        else if (!value.empty())
            sendToUser(user, std::string("Invalid channel name: ") + value);
    }

    for (const auto &channelName : channelNames)
    {
        if(this->channels.find(channelName) != this->channels.end())
        {
            user->leaveChannel(this->channels[channelName]);
        }
    }
    return true;
}

/**
 * @brief Maneja el comando PRIVMSG para enviar mensajes a usuarios o canales.
 *
 * - Separa el `target` y el `msg` de `params`. Limpia el prefijo `:` y
 *   cualquier `CR` final del mensaje.
 * - Si el `target` es un canal (comienza con '#', '&', '+', '!') obtiene
 *   el `Channel` y, si el `user` pertenece al canal, envía el mensaje a
 *   todos los usuarios del canal. Si no pertenece, notifica el error 404.
 * - Si el `target` es un nickname, intenta obtener el `User` por nickname
 *   y enviarle el mensaje.
 *
 * @param user Usuario emisor.
 * @param params Cadena con target y mensaje (ej. "#canal :hola").
 * @return Siempre devuelve true.
 *
 * @note Actualmente no se comprueba si `getUserByNickname` devuelve nullptr
 *       antes de llamar a `sendToUser` (posible mejora).
 */
bool Server::handlePRIVMSG(User *user, const std::string &params)
{
    size_t sp = params.find(' ');
    std::string target = (sp == std::string::npos) ? params : params.substr(0, sp);

    std::string msg;
    if (sp != std::string::npos) {
        msg = params.substr(sp + 1);            // -> ":HOLA" (posible espacio inicial)
        if (!msg.empty() && msg[0] == ' ') msg.erase(0,1);
        if (!msg.empty() && msg[0] == ':') msg.erase(0,1); // quita la ':' sintáctica
        if (!msg.empty() && msg.back() == '\r') msg.pop_back(); // limpia CR si existe
    } else {
        // manejar error: faltan parámetros
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }
    if(target.empty() || msg.empty()) {
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }                   

    std::string out = ":" + user->getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
    Channel *channel = nullptr;
    
    if(target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!') {
        channel = getChannel(target);
        if(channel->hasUser(user)) {
            sendToChannel(channel, out);
        } else {
            sendToUser(user, std::string("404 ") + target + " :Cannot send to channel");
        }
    } else {
        sendToUser(this->getUserByNickname(target), out);
    }
    return true;
}

/**
 * @brief Responde a comandos desconocidos con el código de error 421.
 *
 * @param user Usuario que envió el comando desconocido.
 * @param command Texto del comando no reconocido.
 * @return Siempre devuelve true.
 */
bool Server::handleUnknownCommand(User *user, const std::string &command)
{
    sendToUser(user, std::string("421 ") + command + " :Unknown command");
    return true;
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
