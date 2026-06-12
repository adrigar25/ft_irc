/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelRoles.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:27 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/12 14:56:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <iostream>
#include <string>

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
        throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
    std::map<std::string, std::map<int, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    if(roles.find(role) == roles.end())
        throw IrcException(IRC_ERR_ROLE_NOT_FOUND, "Role not found");
    roles[role]->insert(std::make_pair(fd, user));
}

/**
 * @brief Quita el rol de un `user` 
 *
 * Soporta roles: "operator", "voice", "invited", "banned", "excepted".
 * Lanza `roleNotFoundException` si el rol es inválido.
 */
void Channel::removeRole(User *user, std::string role)
{
    int fd = user->getSocket();
    if(this->users.find(fd) == this->users.end())
        throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
    
    std::map<std::string, std::map<int, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    if(roles.find(role) == roles.end())
        throw IrcException(IRC_ERR_ROLE_NOT_FOUND, "Role not found");
    
    roles[role]->erase(fd);
}