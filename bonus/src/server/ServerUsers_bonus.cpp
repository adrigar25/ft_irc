/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUsers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:10 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:22:39 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server_bonus.hpp"
#include "Channel_bonus.hpp"
#include "User_bonus.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

/**
 * @brief Adds a new user to the server.
 *
 * - Adds the user to the server's user list.
 *
 * @param user Pointer to the User object to be added.
 */
void Server::addUser(User* user)
{
	this->services.users().add(user);
}

/**
 * @brief Deletes a user from the server
 * @param fd The file descriptor of the user to delete
 */
void Server::deleteUser(int fd)
{
	const User *user = this->services.users().findByFd(fd);
	if (!user)
		return;

	for (std::map<std::string, Channel *>::const_iterator it = user->getChannels().begin(); it != user->getChannels().end(); ++it)
	{
		Channel *channel = it->second;
		if (channel)
			channel->deleteUser(user);
		if (channel->isEmpty())
			this->services.channels().deleteChannel(channel->getName());
	}
	this->services.users().remove(fd);
	close(fd);
}

/**
 * @brief Gets a user by their file descriptor
 * @param fd The file descriptor of the user to find
 * @return Pointer to the User object, or nullptr if not found
 */
User* Server::getUserByFd(int fd)
{
	return this->services.users().findByFd(fd);
}
