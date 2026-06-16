/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ServerSignals.cpp								  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:34:09 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 13:15:37 by agarcia		  ###   ########.fr	   */
/*																			*/
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
