/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientRead.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:48 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/22 18:30:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "LineUtils.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>
#include <vector>
#include <map>

static void sendQuitMessageToChannels(Server *server, User *user)
{
	if (!server || !user)
		return;
	const std::map<std::string, Channel *> &channels = user->getChannels();
	while (!channels.empty())
	{
		Channel *channel = user->getChannels().begin()->second;

		std::string quitMessage =
			":" + server->getServices().getUserPrefix(user) +
			"@" + server->getHostname() +
			" QUIT :Client disconnected\r\n";

		server->getServices().sendToChannel(channel, quitMessage, user);

		user->leaveChannel(channel);

		if (channel->isEmpty())
			server->getServices().channels().deleteChannel(channel->getName());
	}
}

bool Server::handleClientRead(int idx)
{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;

	struct pollfd &pfd = this->fds[idx];

	// 💥 PRIORIDAD: errores de socket
	if (pfd.revents & (POLLHUP | POLLERR | POLLNVAL))
	{
		sendQuitMessageToChannels(this, getUserByFd(pfd.fd));
		handleDisconnectionByIndex(idx);
		return true;
	}

	// nada que leer
	if (!(pfd.revents & POLLIN))
		return false;

	int fd = pfd.fd;
	User *client = getUserByFd(fd);

	if (!client)
	{
		std::cout << "[QUIT] - fd: " << fd << "  : Disconnected" << std::endl;
		sendQuitMessageToChannels(this, getUserByFd(fd));
		handleDisconnectionByIndex(idx);
		return true;
	}

	char buffer[1024];

	ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (n <= 0)
	{
		sendQuitMessageToChannels(this, getUserByFd(fd));
		handleDisconnectionByIndex(idx);
		return true;
	}

	buffer[n] = '\0';

	client->getInBuffer().append(buffer, n);

	processClientBuffer(client);

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
