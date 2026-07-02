/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   IRCConnection.cpp								  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:10:43 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 00:57:22 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "IrcConnection_bonus.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

#include <string>

IRCConnection::IRCConnection(): sockfd(-1) {}

IRCConnection::IRCConnection(const IRCConnection &other): sockfd(other.sockfd) {}

IRCConnection &IRCConnection::operator=(const IRCConnection &other)
{
	if (this != &other) {
		this->sockfd = other.sockfd;
	}
	return *this;
}

IRCConnection::~IRCConnection()
{
	if (this->sockfd >= 0)
		close(this->sockfd);
}

int IRCConnection::connectTo(const std::string &host, int port)
{
	struct addrinfo hints;
	struct addrinfo *res = NULL;
	
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	std::ostringstream oss;
	oss << port;
	std::string sport = oss.str();
	if (getaddrinfo(host.c_str(), sport.c_str(), &hints, &res) != 0)
		return -1;
	
	int sock = -1;
	
	for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sock < 0)
			continue;
		if (connect(sock, p->ai_addr, p->ai_addrlen) == 0)
			break;
		close(sock);
		sock = -1;
	}
	freeaddrinfo(res);
	if (sock < 0)
		return -1;
	this->sockfd = sock;
	return sock;
}

size_t IRCConnection::sendRaw(const std::string &s)
{
	if (this->sockfd < 0) return -1;
	std::string m = s + "\r\n";
	return send(this->sockfd, m.c_str(), m.size(), 0);
}

std::string IRCConnection::recvLine()
{
	std::string out;
	char c;
	while (recv(this->sockfd, &c, 1, 0) > 0) {
		out.push_back(c);
		if (c == '\n') break;
	}
	return out;
}

void IRCConnection::closeConn()
{
	if (this->sockfd >= 0) {
		close(this->sockfd);
		this->sockfd = -1;
	}
}

bool IRCConnection::isConnected() const 
{
	return this->sockfd >= 0;
}

int IRCConnection::getFd() const { return this->sockfd; }
