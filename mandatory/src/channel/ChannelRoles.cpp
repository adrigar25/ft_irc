/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelRoles.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:38 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:40:34 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <iostream>
#include <string>

/**
 * @brief Gets the map for a specific role.
 * @param channel The channel for which to get the role map.
 * @param role The role for which to get the map.
 * @return A pointer to the map for the specified role, or NULL if the role is invalid.
 */
static std::map<int, User*> *getRoleMap(Channel *channel, const std::string &role)
{
	if (role == "operator")
		return &channel->getOperators();
	else if (role == "voice")
		return &channel->getVoiceUsers();
	else if (role == "invited")
		return &channel->getInvitedUsers();
	else if (role == "banned")
		return &channel->getBannedUsers();
	else
		return NULL;
}

/**
 * @brief Cambia el rol de un `user` añadiéndolo al conjunto correspondiente.
 *
 * Soporta roles: "operator", "voice", "invited", "banned", "excepted".
 * Lanza `roleNotFoundException` si el rol es inválido.
 */
void Channel::changeRole(User *user, const std::string &role)
{
	int fd = user->getSocket();
	if (this->users.find(fd) == this->users.end())
		throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
	std::map<int, User*> *roleMap = getRoleMap(this, role);

	if (!roleMap)
		throw IrcException(IRC_ERR_ROLE_NOT_FOUND, "Role not found");

	roleMap->insert(std::make_pair(fd, user));
}

/**
 * @brief Quita el rol de un `user`
 *
 * Soporta roles: "operator", "voice", "invited", "banned", "excepted".
 * Lanza `roleNotFoundException` si el rol es inválido.
 */
void Channel::removeRole(const User *user, const std::string &role)
{
	int fd = user->getSocket();
	if (this->users.find(fd) == this->users.end())
		throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
	std::map<int, User*> *roleMap = getRoleMap(this, role);
	if (!roleMap)
		throw IrcException(IRC_ERR_ROLE_NOT_FOUND, "Role not found");
	roleMap->erase(fd);
}
