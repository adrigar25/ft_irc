/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 22:27:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/03 17:13:09 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <iostream>
#include "IrcConnection.hpp"
#include "ChannelManager.hpp"
#include "RequestHandler.hpp"

#define BOT_DEFAULT_NICK "MiBot"
#define BOT_DEFAULT_HOST "127.0.0.1"
#define BOT_DEFAULT_PORT "6667"
#define BOT_DEFAULT_PASSWORD ""

class Bot
{
	private:
		std::string nick;
		std::string host;
		std::string password;
		IRCConnection *conn;
		ChannelManager *cm;
		RequestHandler *rh;
	public:
		int port;
		Bot(const std::string &nick = BOT_DEFAULT_NICK, const std::string &host = BOT_DEFAULT_HOST, const std::string &password = BOT_DEFAULT_PASSWORD);
		~Bot();
		std::string getNick() const;
		std::string getHost() const;
		std::string getPassword() const;
		void setPort(int p);
		int connectToServer();
		void run();
};

#endif
