/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client_error.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: autopatch <autopatch@local>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 01:15:00 by autopatch         #+#    #+#             */
/*   Updated: 2026/05/17 01:15:00 by autopatch        ###   ########.fr       */
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
