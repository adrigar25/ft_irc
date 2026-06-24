/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEvents.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:00:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 18:45:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>


int Server::performPoll()
{
	int nfds = this->fds.size();
	int ready = poll(this->fds.data(), nfds, -1);
	if (ready < 0) {
		if (errno == EINTR) return 0;
		std::cerr << "poll error: " << strerror(errno) << std::endl;
		return -1;
	}
	return ready;
}

void Server::handleClientEvents()
{
	for (int i = 1; i < (int)this->fds.size(); ++i)
	{
		if (handleClientWrite(i) || handleClientRead(i) || handleClientError(i))
		{
			i--;
			continue;
		}
	}
}

void Server::handleEvents()
{
	int ready = 0;

	while (this->running)
	{
		if (this->fds.empty())
		{
			usleep(100000);
			continue;
		}
		ready = performPoll();
		if (ready < 0)
			break;

		if (ready == 0)
			continue;

		if (!this->fds.empty() && (this->fds[0].revents & POLLIN))
		{
			try {
				handleNewConnection();
			} catch (const std::exception &e) {
				std::cerr << "Accept error: " << e.what() << std::endl;
			}
		}

		try {
			handleClientEvents();
		} catch (const std::exception &e) {
			std::cerr << "Client error: " << e.what() << std::endl;
		}
	}
	cleanup();
}
