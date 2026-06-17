/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUsers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:17 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/18 01:41:47 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>
#include <sstream>

void Server::addUser(User* user)
{
	this->services.users().add(user);
}

void Server::deleteUser(int fd)
{
	User* user = this->services.users().findByFd(fd);
	if (!user) return;
	
	for(std::map<std::string, Channel*>::const_iterator it = user->getChannels().begin(); it != user->getChannels().end(); ++it)
	{
		Channel* channel = it->second;
		if (channel)
			channel->deleteUser(user);
		if (channel->isEmpty())
			this->services.channels().deleteChannel(channel->getName());
	}
	this->services.users().remove(fd);
	close(fd);
}

User* Server::getUserByFd(int fd)
{
	return this->services.users().findByFd(fd);
}
