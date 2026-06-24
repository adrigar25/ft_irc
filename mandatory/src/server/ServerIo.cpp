/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:28 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 18:01:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "SocketUtils.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <cctype>
#include <cstring>
#include <cstdio>

static std::string prepareOutMessage(const User *user, const std::string &message)
{

	if (!message.empty())
	{
		if (message[0] == ':')
			return message;

		if (message.size() >= 3 && std::isdigit((unsigned char)message[0]) && std::isdigit((unsigned char)message[1]) && std::isdigit((unsigned char)message[2]) && (message.size() == 3 || message[3] == ' '))
			return message;

		const char *cmds[] = {"PRIVMSG", "NOTICE", "JOIN", "PART", "MODE", "KICK", "INVITE", "QUIT", "NICK", "USER", "PASS", "PING", "PONG", "ERROR", "CAP"};

		for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); ++i)
		{
			size_t len = std::strlen(cmds[i]);
			if (message.compare(0, len, cmds[i]) == 0 &&
				(message.size() == len || message[len] == ' '))
				return message;
		}
	}

	return std::string("NOTICE ") + user->getNickname() + " :" + message;
}

void Server::enqueuePending(User *user, const char *buf, size_t len)
{
	std::string &pending = user->getOutBuffer();
	pending.append(buf, len);
	enablePollOutForFd(user->getSocket());
}

void Server::sendToUser(User *user, const std::string &message)
{
	std::string msg = prepareOutMessage(user, message);
	msg = formatMessage(msg);

	int fd = user->getSocket();

	std::string &pending = user->getOutBuffer();
	size_t &offset = user->getOutOffset();

	if (!pending.empty() || offset != 0)
	{
		pending.append(msg);
		enablePollOutForFd(fd);
		return;
	}

	ssize_t sent = send(fd, msg.c_str(), msg.size(), 0);

	if (sent <= 0)
	{
		handleDisconnectionByFd(fd);
		return;
	}

	if (sent < (ssize_t)msg.size())
	{
		pending = msg.substr(sent);
		offset = 0;
		enablePollOutForFd(fd);
	}
}


void Server::sendToChannel(Channel *channel, const std::string &message, const User *exclude)
{
	const std::map<int, User*>& usersMap = channel->getUsers();
	for (std::map<int, User*>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
	{
		if (it->second && it->second != exclude)
			sendToUser(it->second, message);
	}
}

void Server::enablePollOutForFd(int fd)
{
	for (size_t i = 0; i < this->fds.size(); ++i) {
		if (this->fds[i].fd == fd) {
			this->fds[i].events |= POLLOUT;
			return;
		}
	}
	pushPollFd(fd, POLLIN | POLLOUT);
}
