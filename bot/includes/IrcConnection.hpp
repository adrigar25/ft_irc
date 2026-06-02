/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:15:52 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:15:53 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCCONNECTION_HPP
#define IRCCONNECTION_HPP

#include <string>

class IRCConnection {
public:
	IRCConnection();
	~IRCConnection();

	// Connects to host:port, returns socket fd or -1 on error
	int connectTo(const std::string &host, int port);
	// Send a raw IRC line (adds CRLF)
	ssize_t sendRaw(const std::string &s);
	// Receive a single line (including trailing LF), or empty on disconnect
	std::string recvLine();
	void closeConn();
	bool isConnected() const;
	int getFd() const;

private:
	int sockfd;
};

#endif

