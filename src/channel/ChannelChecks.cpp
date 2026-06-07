/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelChecks.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:24 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 15:22:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include <map>

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
 * @brief Comprueba si el canal no tiene usuarios.
 */
bool Channel::isEmpty() const
{
    return this->userCount == 0;
}


/**
 * @brief Comprueba si `user` puede unirse al canal según límites, privacy y baneos.
 */
bool Channel::canUserJoin(User *user) const
{
    if (this->getUserLimit() != -1 && this->getUserCount() >= this->getUserLimit())
        return false;
    if(this->getIsInviteOnly() && !this->isUserInvited(user))
        return false;
    if(this->isUserBanned(user))
        return false;
    return true;
}
