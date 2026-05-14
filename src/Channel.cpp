/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:15 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:53:28 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include <map>
#include <iostream>

/**
 * @brief Constructor de `Channel`.
 *
 * Crea un canal con el nombre `name`, añade al `creator` como usuario
 * y lo marca como operador.
 * @param name Nombre del canal.
 * @param creator Usuario que crea el canal.
 */
Channel::Channel(std::string name, User *creator): name(name), userCount(0), isPrivate(false), userLimit(-1)
{
    std::cout << "Channel " << this->name << " created by " << creator->getNickname() << std::endl;
    this->addUser(creator);
    this->addUserToMap(this->operators, creator);
}


/**
 * @brief Destructor de `Channel`.
 */
Channel::~Channel()
{
    std::cout << "Channel " << this->name << " deleted" << std::endl;
};


/**
 * @brief Añade un usuario al canal, validando restricciones (privado, límite, baneos).
 *
 * Lanza excepciones específicas si el usuario no puede unirse.
 * @param user Puntero al `User` a añadir.
 */
void Channel::addUser(User *user)
{
    int fd = user->getSocket();
    if (this->isPrivate && this->invitedUsers.find(fd) == this->invitedUsers.end())
        throw privateChannelException();
    if (this->users.find(fd) != this->users.end())
        throw userAlreadyExistsException();
    if (this->userCount >= this->userLimit && this->userLimit != -1)
        throw channelFullException();
    if (this->bannedUsers.find(fd) != this->bannedUsers.end())
        throw userBannedException();
    this->users.insert(std::make_pair(fd, user));
    this->userCount++;
    std::cout << "User " << user->getNickname() << " added to channel " << this->name << std::endl;
}


/**
 * @brief Elimina un usuario del canal y actualiza el contador.
 * @param user Puntero al `User` a eliminar.
 */
void Channel::deleteUser(User *user)
{
    int fd = user->getSocket();
    if (this->users.find(fd) == this->users.end())
        throw userNotFoundException();
    this->users.erase(fd);
    this->userCount--;
    std::cout << "User " << user->getNickname() << " removed from channel " << this->name << std::endl;
}


/**
 * @brief Añade `user` al mapa `userMap` si no existe.
 */
void Channel::addUserToMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) != userMap.end())
        return;
    userMap.insert(std::make_pair(fd, user));
}


/**
 * @brief Elimina `user` del mapa `userMap` si existe.
 */
void Channel::deleteUserFromMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) == userMap.end())
        return;
    userMap.erase(fd);
}


/**
 * @brief Cambia el rol de un `user` añadiéndolo al conjunto correspondiente.
 *
 * Soporta roles: "operator", "voice", "invited", "banned", "excepted".
 * Lanza `roleNotFoundException` si el rol es inválido.
 */
void Channel::changeRole(User *user, std::string role)
{
    int fd = user->getSocket();
    if(this->users.find(fd) == this->users.end())
        throw userNotFoundException();
    std::map<std::string, std::map<int, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    roles.insert(std::make_pair(std::string("excepted"), &this->exceptedUsers));

    if(roles.find(role) == roles.end())
        throw roleNotFoundException();
    roles[role]->insert(std::make_pair(fd, user));
}


/**
 * @brief Devuelve el número de usuarios en el canal.
 */
int Channel::getUserCount() const
{
    return this->userCount;
}


/**
 * @brief Devuelve el límite de usuarios del canal, o -1 si no hay límite.
 */
int Channel::getUserLimit() const
{
    return this->userLimit;
}


/**
 * @brief Indica si el canal es privado.
 */
bool Channel::getIsPrivate() const
{
    return this->isPrivate;
}


/**
 * @brief Devuelve el nombre del canal.
 */
std::string Channel::getName() const
{
    return this->name;
}


/**
 * @brief Comprueba si `user` pertenece al canal.
 */
bool Channel::hasUser(User *user) const
{
    int fd = user->getSocket();
    return this->users.find(fd) != this->users.end();
}


/**
 * @brief Comprueba si `user` es operador del canal.
 */
bool Channel::isUserOperator(User *user) const
{
    int fd = user->getSocket();
    return this->operators.find(fd) != this->operators.end();
}


/**
 * @brief Comprueba si `user` tiene voz en el canal.
 */
bool Channel::isUserVoice(User *user) const
{
    int fd = user->getSocket();
    return this->voiceUsers.find(fd) != this->voiceUsers.end();
}


/**
 * @brief Comprueba si `user` fue invitado al canal.
 */
bool Channel::isUserInvited(User *user) const
{
    int fd = user->getSocket();
    return this->invitedUsers.find(fd) != this->invitedUsers.end();
}


/**
 * @brief Comprueba si `user` está baneado del canal.
 */
bool Channel::isUserBanned(User *user) const
{
    int fd = user->getSocket();
    return this->bannedUsers.find(fd) != this->bannedUsers.end();
}


/**
 * @brief Comprueba si `user` está en la lista de excepciones del canal.
 */
bool Channel::isUserExcepted(User *user) const
{
    int fd = user->getSocket();
    return this->exceptedUsers.find(fd) != this->exceptedUsers.end();
}


/**
 * @brief Comprueba si `user` puede unirse al canal según límites, privacy y baneos.
 */
bool Channel::canUserJoin(User *user) const
{
    if (this->getUserLimit() != -1 && this->getUserCount() >= this->getUserLimit())
        return false;
    if(this->getIsPrivate() && !this->isUserInvited(user))
        return false;
    if(this->isUserBanned(user))
        return false;
    return true;
}


/**
 * @brief Devuelve el mapa de usuarios del canal.
 */
const std::map<int, User*>& Channel::getUsers() const
{
    return this->users;
}