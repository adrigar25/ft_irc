/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:15:33 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:15:47 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>

class IRCConnection;
class ChannelManager;

class RequestHandler {
public:
	RequestHandler(IRCConnection *conn, ChannelManager *cm, const std::string &nick);
	~RequestHandler();

	void handleLine(const std::string &line);

private:
	IRCConnection *conn;
	ChannelManager *cm;
	std::string nick;

	std::string chooseReply(const std::string &text) const;
};

#endif
