/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:13:33 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:15:17 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "BotOptions.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

static std::string trim(const std::string &s){
	size_t b = s.find_first_not_of(" \r\n");
	if (b==std::string::npos)
		return ("");
	size_t e = s.find_last_not_of(" \r\n");
	return (s.substr(b, e-b+1));
}

Bot::Bot(const std::string &nick, const std::string &host , const std::string &password)
	: nick(nick), host(host), password(password), conn(nullptr), cm(nullptr), rh(nullptr), port(std::atoi(BOT_DEFAULT_PORT))
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	this->conn = new IRCConnection();
	this->cm = new ChannelManager(this->conn);
	this->rh = new RequestHandler(this->conn, this->cm, this->nick);
}

Bot::~Bot()
{
	if (this->rh)
		delete (this->rh);
	if (this->cm)
		delete (this->cm);
	if (this->conn)
		delete (this->conn);
}

std::string Bot::getNick() const {
	return (nick);
}

std::string Bot::getHost() const {
	return (host);
}

std::string Bot::getPassword() const {
	return (password);
}

void Bot::setPort(int p) {
	if (p > 0)
		this->port = p;
}

int Bot::connectToServer()
{
	if (!this->conn)
		return (-1);
	return (this->conn->connectTo(this->host, this->port));
}

void Bot::run()
{
	if (!this->conn || !this->conn->isConnected()){
		std::cerr << "Bot: socket not connected\n";
		return;
	}

	this->conn->sendRaw("PASS " + this->password);
	this->conn->sendRaw("NICK " + this->nick);
	this->conn->sendRaw("USER " + this->nick + " 0 * :" + this->nick);

	while (this->conn && this->conn->isConnected()){
		std::string line = this->conn->recvLine();
		if (line.empty()){
			std::cerr << "Conexión cerrada\n";
			break;
		}
		std::string tline = trim(line);
		std::cout << "< " << tline << "\n";

		if (tline.rfind("PING", 0) == 0){
			std::string token = trim(tline.substr(4));
			if (!token.empty() && token[0] == ':')
				token = token.substr(1);
			this->conn->sendRaw("PONG :" + token);
			continue;
		}

		if (this->rh)
			this->rh->handleLine(line);
	}
}
