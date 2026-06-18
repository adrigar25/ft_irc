/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:28 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/19 00:57:47 by agarcia          ###   ########.fr       */
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

static std::string prepareOutMessage(User *user, const std::string &message)
{
	std::string msg = message;

	bool needsWrap = true;
	if (!msg.empty()) {
		if (msg[0] == ':')
			needsWrap = false;
		else if (msg.size() >= 3 && std::isdigit((unsigned char)msg[0]) && std::isdigit((unsigned char)msg[1]) && std::isdigit((unsigned char)msg[2]) && (msg.size() == 3 || msg[3] == ' '))
			needsWrap = false;
		else {
			const char *cmds[] = {"PRIVMSG","NOTICE","JOIN","PART","MODE","KICK","INVITE","QUIT","NICK","USER","PASS","PING","PONG","ERROR","CAP"};
			for (size_t i = 0; i < sizeof(cmds)/sizeof(cmds[0]); ++i) {
				size_t len = std::strlen(cmds[i]);
				if (msg.size() >= len && msg.compare(0, len, cmds[i]) == 0 && (msg.size() == len || msg[len] == ' ')) {
					needsWrap = false;
					break;
				}
			}
		}
	}
	if (needsWrap) {
		std::string notice = std::string("NOTICE ") + user->getNickname() + " :" + msg;
		msg.swap(notice);
	}
	return msg;
}

static std::string escapeForLog(const std::string &msg)
{
	std::string escaped;
	escaped.reserve(msg.size() * 3 + 10);
	for (size_t i = 0; i < msg.size(); ++i) {
		unsigned char c = static_cast<unsigned char>(msg[i]);
		if (c == '\r') escaped += "\\r";
		else if (c == '\n') escaped += "\\n";
		else if (c >= 32 && c < 127) escaped += msg[i];
		else {
			char buf[8];
			std::snprintf(buf, sizeof(buf), "\\x%02x", c);
			escaped += buf;
		}
	}
	return escaped;
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


void Server::sendToChannel(Channel *channel, const std::string &message, User *exclude)
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
