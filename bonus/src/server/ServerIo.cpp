/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ServerIo.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:38:51 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/15 17:13:43 by agarcia		  ###   ########.fr	   */
/*																			*/
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

static bool isNumericCode(const std::string &msg)

{

	return (msg.size() >= 3 &&

		std::isdigit((unsigned char)msg[0]) &&

		std::isdigit((unsigned char)msg[1]) &&

		std::isdigit((unsigned char)msg[2]) &&

		(msg.size() == 3 || msg[3] == ' '));

}

static bool isIrcCommand(const std::string &msg)

{

	const char *cmds[] = {

		"PRIVMSG", "NOTICE", "JOIN", "PART", "MODE",

		"KICK", "INVITE", "QUIT", "NICK", "USER",

		"PASS", "PING", "PONG", "ERROR", "CAP"

	};

	for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++)

	{

		size_t len = std::strlen(cmds[i]);

		if (msg.compare(0, len, cmds[i]) == 0 &&

			(msg.size() == len || msg[len] == ' '))

			return true;

	}

	return false;

}

static bool isAlreadyIrcFormatted(const std::string &msg)

{

	if (msg.empty())

		return false;

	if (msg[0] == ':')

		return true;

	if (isNumericCode(msg))

		return true;

	return isIrcCommand(msg);

}

static std::string prepareOutMessage(User *user, const std::string &message)
{
	if (isAlreadyIrcFormatted(message))
		return message;

	return "NOTICE " + user->getNickname() + " :" + message;
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

static ssize_t performSend(int fd, const char *buf, size_t len)
{
	size_t total = 0;
	while (total < len) {
		ssize_t n = send(fd, buf + total, len - total, 0);
		if (n > 0) {
			total += static_cast<size_t>(n);
			continue;
		}
		if (n == 0) {
			return static_cast<ssize_t>(total);
		}
		if (errno == EINTR)
			continue;
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return static_cast<ssize_t>(total);
		}
		return -1;
	}
	return static_cast<ssize_t>(total);
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

	std::string escaped = escapeForLog(msg);

	std::string &pending = user->getOutBuffer();
	size_t &offset = user->getOutOffset();

	if (!pending.empty() || offset != 0)
	{
		enqueuePending(user, msg.c_str(), msg.size());
		return;
	}

	const char *buf = msg.c_str();
	size_t len = msg.size();

	ssize_t sent = performSend(fd, buf, len);
	
	if (sent < 0) {
		std::cerr << "send failed: " << strerror(errno) << " fd=" << fd << std::endl;
		return;
	}
	
	if(sent < static_cast<ssize_t>(len))
		enqueuePending(user, buf + sent, len - sent);
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
