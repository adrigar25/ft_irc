/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ServerClientWrite.cpp							  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:38:42 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:38:42 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>

bool Server::handleClientWrite(int idx)
{
	if (idx < 0 || idx >= (int)this->fds.size())
		return false;

	if (!(this->fds[idx].revents & POLLOUT))
		return false;

	int fd = this->fds[idx].fd;
	User *user = this->services.users().findByFd(fd);
	if (!user)
	{
		handleDisconnectionByIndex(idx);
		return true;
	}

	std::string &out = user->getOutBuffer();
	size_t &offset = user->getOutOffset();

	if (out.empty())
	{
		this->fds[idx].events &= ~POLLOUT;
		return false;
	}

	ssize_t n = send(fd, out.data() + offset, out.size() - offset, 0);

	if (n <= 0)
	{
		handleDisconnectionByIndex(idx);
		return true;
	}

	offset += (size_t)n;

	if (offset >= out.size())
	{
		out.clear();
		offset = 0;
		this->fds[idx].events &= ~POLLOUT;
	}

	return false;
}

