/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:09:03 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:12:39 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "IrcConnection.hpp"
#include "ChannelManager.hpp"
#include "BotOptions.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string &s)
{
	size_t a = 0;
	size_t b = s.size();
	while (a < b && std::isspace((unsigned char)s[a]))
		++a;
	while (b > a && std::isspace((unsigned char)s[b-1]))
		--b;
	return (s.substr(a, b-a));
}

RequestHandler::RequestHandler(IRCConnection *conn, ChannelManager *cm, const std::string &nick)
: conn(conn), cm(cm), nick(nick) {}

RequestHandler::~RequestHandler() {}

std::string RequestHandler::chooseReply(const std::string &text) const
{
	std::string t = text;
	// lowercase
	std::transform(t.begin(), t.end(), t.begin(), ::tolower);
	if (t.find(BOT_TRIGGER_1) != std::string::npos)
		return (BOT_RESPONSE_1);
	if (t.find(BOT_TRIGGER_2) != std::string::npos)
		return (BOT_RESPONSE_2);
	return ("");
}

void RequestHandler::handleLine(const std::string &line)
{
	// Parse: [:prefix] COMMAND params :trail
	std::string raw = trim(line);
	if (raw.empty())
		return;
	std::string prefix;
	std::string rest = raw;
	if (rest[0] == ':'){
		size_t sp = rest.find(' ');
		if (sp == std::string::npos)
			return;
		prefix = rest.substr(1, sp-1);
		rest = rest.substr(sp+1);
	}
	std::istringstream iss(rest);
	std::string cmd;
	iss >> cmd;
	if (cmd == "INVITE"){
		std::string target, chan;
		iss >> target >> chan;
		if (target == this->nick){
			if (this->cm)
				this->cm->joinChannel(chan);
			if (this->cm)
				this->cm->sendToChannel(chan, "Gracias por invitarme! Me uno.");
		}
		return;
	}
	if (cmd == "PRIVMSG"){
		std::string dest;
		iss >> dest;
		std::string payload;
		// remainder may start with :
		size_t pos = rest.find(" :");
		if (pos != std::string::npos)
			payload = rest.substr(pos+2);
		std::string sender = prefix;
		// reduce sender to nick if contains '!'
		size_t excl = sender.find('!');
		if (excl != std::string::npos)
			sender = sender.substr(0, excl);

		if (sender == this->nick)
			return; // ignore own messages

		bool addressed = false;
		if (dest == this->nick)
			addressed = true;
		if (!addressed && payload.find(this->nick) != std::string::npos)
			addressed = true;

		if (addressed){
			std::string reply = chooseReply(payload);
			if (reply.empty())
				reply = "No entiendo...";
			if (dest.size() && (dest[0] == '#' || dest[0] == '&')){
				if (this->cm)
					this->cm->sendToChannel(dest, reply);
			} else {
				if (this->conn)
					this->conn->sendRaw("PRIVMSG " + sender + " :" + reply);
			}
		}
		return;
	}
}
