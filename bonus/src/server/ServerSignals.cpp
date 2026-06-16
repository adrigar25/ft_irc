/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSignals.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:10 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:11 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <csignal>
#include <iostream>

void handleSignalStopServer(int signum)
{
	Server *server = Server::getInstance();
	if (server) {
		(void)signum;
		server->stopServer();
	}
}

void Server::initSignals()
{
	signal(SIGINT, handleSignalStopServer);
	signal(SIGTERM, handleSignalStopServer);
	signal(SIGQUIT, handleSignalStopServer);
}
