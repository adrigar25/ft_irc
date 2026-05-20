/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:32 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 00:18:11 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "User.hpp"
#include "Channel.hpp"
#include <iostream>
#include <exception>

/**
 * @brief Constructor de `User`.
 *
 * Inicializa el descriptor de socket y el nickname, dejando otros campos
 * en estado por defecto (no autenticado, flags no establecidos).
 *
 * @param socket Descriptor de socket del usuario.
 * @param name Nickname inicial del usuario.
 */
User::User(int socket, const std::string &name)
        : socket(socket), nickname(name), username(""),
            inBuffer(), outBuffer(), outOffset(0),
            nickSet(false), userSet(false), passGiven(false), authenticated(false) {}


/**
 * @brief Destructor de `User`.
 */
User::~User() {}


/**
 * @brief Devuelve el nickname del usuario.
 * @return `std::string` con el nickname.
 */
std::string User::getNickname() const
{
    return this->nickname;
}


/**
 * @brief Devuelve el descriptor de socket del usuario.
 * @return `int` descriptor de socket.
 */
int User::getSocket() const
{
    return this->socket;
}


/**
 * @brief Devuelve el mapa de canales en los que está el usuario.
 * @return Referencia constante al mapa de `Channel*` indexado por nombre.
 */
const std::map<std::string, Channel*>& User::getChannels() const
{
    return this->channels;
}


/**
 * @brief Establece el nickname del usuario y marca `nickSet`.
 * @param name Nuevo nickname.
 */
void User::setNickname(const std::string &name)
{
    this->nickname = name;
    this->nickSet = true;
}


/**
 * @brief Establece el nombre de usuario (username) y marca `userSet`.
 * @param name Nombre de usuario.
 */
void User::setUsername(const std::string &name)
{
    this->username = name;
    this->userSet = true;
}


/**
 * @brief Devuelve el `username` del usuario.
 * @return `std::string` con el username.
 */
std::string User::getUsername() const
{
    return this->username;
}


/**
 * @brief Marca si el usuario proporcionó la contraseña (PASS).
 * @param val `true` si la contraseña fue proporcionada.
 */
void User::setPass(bool val)
{
    this->passGiven = val;
}


/**
 * @brief Comprueba si el PASS fue establecido.
 * @return `true` si PASS fue proporcionado.
 */
bool User::isPassSet() const
{
    return this->passGiven;
}


/**
 * @brief Comprueba si el nickname fue establecido.
 * @return `true` si el nick está establecido.
 */
bool User::isNickSet() const
{
    return this->nickSet;
}


/**
 * @brief Comprueba si el username fue establecido.
 * @return `true` si el username está establecido.
 */
bool User::isUserSet() const
{
    return this->userSet;
}


/**
 * @brief Marca el estado de autenticación del usuario.
 * @param val `true` si el usuario está autenticado.
 */
void User::setAuthenticated(bool val)
{
    this->authenticated = val;
}


/**
 * @brief Comprueba si el usuario está autenticado.
 * @return `true` si está autenticado.
 */
bool User::isAuthenticated() const
{
    return this->authenticated;
}


/**
 * @brief Añade al usuario al `channel` y actualiza estructuras.
 *
 * Lanza `userAlreadyInChannelException` si ya está presente. Inserta
 * el canal en el mapa local y llama a `Channel::addUser`.
 * @param channel Canal al que unirse.
 */
void User::joinChannel(Channel *channel)
{
    if(this->channels.find(channel->getName()) != this->channels.end())
        throw userAlreadyInChannelException();

    this->channels.insert(std::make_pair(channel->getName(), channel));
    channel->addUser(this);
}


/**
 * @brief Abandona el `channel` llamando a `Channel::deleteUser`.
 * @param channel Canal a abandonar.
 */
void User::leaveChannel(Channel *channel)
{
    channel->deleteUser(this);
}


/* Buffer accessors */
std::string& User::getInBuffer()
{
    return this->inBuffer;
}

std::string& User::getOutBuffer()
{
    return this->outBuffer;
}

size_t& User::getOutOffset()
{
    return this->outOffset;
}

void User::appendToOutBuffer(const std::string &data)
{
    this->outBuffer.append(data);
}