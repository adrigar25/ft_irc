/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:21:29 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:21:31 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <poll.h>
#include <sys/socket.h>

bool Server::handleClientError(int idx)
{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;
	if (this->fds[idx].revents & (POLLHUP | POLLERR | POLLNVAL))
	{
		handleDisconnectionByIndex(idx);
		return true;
	}
	return false;
}
