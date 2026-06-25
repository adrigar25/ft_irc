/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelUsers.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:19:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/25 18:21:26 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <algorithm>

/**
 * @brief Añade un usuario al canal, validando restricciones (invite-only, límite, baneos).
 *
 * Lanza excepciones específicas si el usuario no puede unirse.
 * @param user Puntero al `User` a añadir.
 */
void Channel::addUser(User *user)
{
	int fd = user->getSocket();
	this->users.insert(std::make_pair(fd, user));
	this->userCount++;
}

/**
 * @brief Elimina un usuario del canal y actualiza el contador.
 * @param user Puntero al `User` a eliminar.
 */
void Channel::deleteUser(const User *user)
{
	int fd = user->getSocket();
	if (this->users.find(fd) == this->users.end())
		throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
	this->users.erase(fd);
	this->userCount--;
}

/**
 * @brief Añade `user` al mapa `userMap` si no existe.
 */
void Channel::addUserToMap(std::map<int, User *> &userMap, User *user)
{
	int fd = user->getSocket();
	if (userMap.find(fd) != userMap.end())
		return;
	userMap.insert(std::make_pair(fd, user));
}

/**
 * @brief Elimina `user` del mapa `userMap` si existe.
 */
void Channel::deleteUserFromMap(std::map<int, User *> &userMap, const User *user)
{
	int fd = user->getSocket();
	if (userMap.find(fd) == userMap.end())
		return;
	userMap.erase(fd);
}

/**
 * @brief Marca a `user` como invitado en el canal.
 */
void Channel::inviteUser(User *user)
{
	addUserToMap(this->invitedUsers, user);
}

void Channel::removeInvitedUser(const User *user)
{
	deleteUserFromMap(this->invitedUsers, user);
}

void Channel::banMask(const std::string &mask)
{
	if (std::find(this->bannedMasks.begin(), this->bannedMasks.end(), mask) == this->bannedMasks.end())
		this->bannedMasks.push_back(mask);
}

void Channel::unbanMask(const std::string &mask)
{
	std::vector<std::string>::iterator it = std::find(this->bannedMasks.begin(), this->bannedMasks.end(), mask);
	if (it != this->bannedMasks.end())
		this->bannedMasks.erase(it);
}

void Channel::unbanMatchingMask(const std::string &userMask)
{
	for (std::vector<std::string>::iterator it = this->bannedMasks.begin(); it != this->bannedMasks.end(); )
	{
		if (fnmatch(it->c_str(), userMask.c_str(), 0) == 0)
			it = this->bannedMasks.erase(it);
		else
			++it;
	}
}
