/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientRead.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:48 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/18 02:00:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "LineUtils.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>
#include <vector>

static ssize_t recvFromFd(int fd, char *buffer, size_t buflen)
{
	ssize_t n;
	while (true)
	{
		n = recv(fd, buffer, buflen, 0);
		if (n < 0 && errno == EINTR)
			continue;
		break;
	}
	return n;
}

bool Server::handleClientRead(int idx)

{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;
	if (!(this->fds[idx].revents & POLLIN))
		return false;

	int fd = this->fds[idx].fd;
	User *user = getUserByFd(fd);

	if (!user)
	{
		handleDisconnectionByIndex(idx);
		return true;
	}

	char buffer[1024];
	ssize_t n = recvFromFd(fd, buffer, sizeof(buffer) - 1);
	if (n <= 0)
	{
		handleDisconnectionByIndex(idx);
		return true;
	}
	buffer[n] = '\0';

	user->getInBuffer().append(buffer, n);
	Server::processClientBuffer(user);
	return false;
}

void Server::processClientBuffer(User *user)
{
	if (!user)
		return;

	std::string &buffer = user->getInBuffer();
	std::vector<std::string> lines;
	popLines(buffer, lines);

	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		try
		{
			this->handleClientCommand(user, *it);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Client error fd " << user->getSocket() << ": " << e.what() << std::endl;
			this->handleDisconnectionByFd(user->getSocket());
			break;
		}
	}
}
