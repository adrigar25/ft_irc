/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerAccept.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:09 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 16:36:45 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "SocketUtils.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void Server::handleNewConnection()
{
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen = sizeof(clientAddress);
	int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
	if (newSocket < 0)
		throw IrcException(IRC_ERR_ACCEPTING_CONNECTION, std::string("accept failed: ") + strerror(errno));

	setSocketNonBlocking(newSocket);
	pushPollFd(newSocket, POLLIN);

	User* newUser = new User(newSocket, "*");
	addUser(newUser);

}

void Server::handleDisconnectionByIndex(int idx)
{
	if (idx <= 0 || idx >= (int)this->fds.size()) {
		std::cerr << "handleDisconnectionByIndex: invalid idx " << idx << std::endl;
		return;
	}
	int fd = this->fds[idx].fd;

	deleteUser(fd);
	this->fds.erase(this->fds.begin() + idx);
}

void Server::handleDisconnectionByFd(int fd)
{
	for (size_t i = 1; i < this->fds.size(); ++i) {
		if (this->fds[i].fd == fd) {
			handleDisconnectionByIndex(static_cast<int>(i));
			return;
		}
	}
	std::cerr << "handleDisconnectionByFd: fd not found " << fd << std::endl;
}
