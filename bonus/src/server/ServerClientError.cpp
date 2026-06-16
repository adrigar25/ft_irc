/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:38 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:38:38 by agarcia          ###   ########.fr       */
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
