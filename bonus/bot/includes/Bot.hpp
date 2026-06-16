/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 22:27:25 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/15 16:41:04 by agarcia          ###   ########.fr       */
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
#define BOT_DEFAULT_PORT 6667
#define BOT_DEFAULT_PASSWORD ""

class Bot
{
	private:
		std::string host;
		int port;
		std::string password;
		std::string nick;
		IRCConnection *irc;
		ChannelManager *cm;
		RequestHandler *rh;
	public:
		Bot(
				const std::string &host = BOT_DEFAULT_HOST,
				const int port = BOT_DEFAULT_PORT,
				const std::string &password = BOT_DEFAULT_PASSWORD,
				const std::string &nick = BOT_DEFAULT_NICK
			);
		~Bot();
		std::string getNick() const;
		std::string getHost() const;
		std::string getPassword() const;
		void setPort(int p);
		int connectToServer();
		void run();
};

#endif
