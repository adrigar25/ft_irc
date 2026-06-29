/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:28 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:21:28 by agarcia          ###   ########.fr       */
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

/**
 * @brief Prepares the output message for a user
 * @param user The user to send the message to
 * @param message The message to send
 * @return The prepared message
 */
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

/**
 * @brief Enqueues a pending message for a user
 * @param user The user to send the message to
 * @param buf The buffer containing the message
 * @param len The length of the message
 */
void Server::enqueuePending(User *user, const char *buf, size_t len)
{
	std::string &pending = user->getOutBuffer();
	pending.append(buf, len);
	enablePollOutForFd(user->getSocket());
}

/**
 * @brief Sends a message to a user
 * @param user The user to send the message to
 * @param message The message to send
 */
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

/**
 * @brief Sends a message to all users in a channel, excluding a specific user
 * @param channel The channel to send the message to
 * @param message The message to send
 * @param exclude The user to exclude from receiving the message
 */
void Server::sendToChannel(Channel *channel, const std::string &message, const User *exclude)
{
	const std::map<int, User*>& usersMap = channel->getUsers();
	for (std::map<int, User*>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
	{
		if (it->second && it->second != exclude)
			sendToUser(it->second, message);
	}
}

/**
 * @brief Enables polling for output on a file descriptor
 * @param fd The file descriptor to enable polling for
 */
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
