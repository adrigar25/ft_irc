/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:09:03 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/04 18:48:33 by agarcia          ###   ########.fr       */
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

static std::string cleanIrcParam(const std::string &s)
{
	std::string value = trim(s);
	if (!value.empty() && value[0] == ':')
		value.erase(0, 1);
	return (value);
}

RequestHandler::RequestHandler(IRCConnection *conn, ChannelManager *cm, const std::string &nick)
: conn(conn), cm(cm), nick(nick) {}

RequestHandler::~RequestHandler() {}

std::string RequestHandler::chooseReply(const std::string &text) const
{
	std::string t = text;
	// lowercase
	std::transform(t.begin(), t.end(), t.begin(), ::tolower);
	for (const BotCmd *cmd = BOT_COMMANDS; cmd->trigger; ++cmd) {
		std::string trig = cmd->trigger;
		std::transform(trig.begin(), trig.end(), trig.begin(), ::tolower);
		if (t.find(trig) != std::string::npos)
			return std::string(cmd->response);
	}
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
		chan = cleanIrcParam(chan);
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
		// direct/private message to the bot
		if (dest == this->nick)
			addressed = true;
		// channel command prefix (e.g. "!hola")
		std::string stripped = trim(payload);
		if (!addressed && !stripped.empty() && (stripped[0] == '!'))
			addressed = true;
		// mention of the bot (case-insensitive)
		if (!addressed) {
			std::string lowerPayload = payload;
			std::string lowerNick = this->nick;
			std::transform(lowerPayload.begin(), lowerPayload.end(), lowerPayload.begin(), ::tolower);
			std::transform(lowerNick.begin(), lowerNick.end(), lowerNick.begin(), ::tolower);
			if (lowerPayload.find(lowerNick) != std::string::npos)
				addressed = true;
		}

		if (addressed){
			// soporte para comando de ayuda: !help
			std::string lowerStripped = stripped;
			std::transform(lowerStripped.begin(), lowerStripped.end(), lowerStripped.begin(), ::tolower);
			if (lowerStripped == "!help" || lowerStripped.rfind("!help ", 0) == 0) {
				std::ostringstream oss;
				oss << "Comandos disponibles:\n";
				oss << "!help - muestra esta ayuda;\n";
				for (const BotCmd *cmd = BOT_COMMANDS; cmd->trigger; ++cmd) {
					oss << "!" << cmd->trigger << " - " << cmd->response;
					if ((cmd + 1)->trigger)
						oss << "; \n";
				}
				std::string helpMsg = oss.str();
				// Send help message line-by-line to avoid embedding '\n' inside a single PRIVMSG
				if (dest.size() && (dest[0] == '#' || dest[0] == '&')){
					if (this->cm) {
						std::istringstream lss(helpMsg);
						std::string lineMsg;
						while (std::getline(lss, lineMsg)) {
							lineMsg = trim(lineMsg);
							if (lineMsg.empty())
								continue;
							this->cm->sendToChannel(dest, lineMsg);
						}
					}
				} else {
					if (this->conn && !sender.empty()){
						std::istringstream lss(helpMsg);
						std::string lineMsg;
						while (std::getline(lss, lineMsg)) {
							lineMsg = trim(lineMsg);
							if (lineMsg.empty())
								continue;
							this->conn->sendRaw("PRIVMSG " + sender + " :" + lineMsg);
						}
					}
				}
			} else {
				std::string reply = chooseReply(payload);
				if (reply.empty())
					reply = "No entiendo...";
				if (dest.size() && (dest[0] == '#' || dest[0] == '&')){
					if (this->cm)
						this->cm->sendToChannel(dest, reply);
				} else {
					if (this->conn && !sender.empty())
						this->conn->sendRaw("PRIVMSG " + sender + " :" + reply);
				}
			}
		}
		return;
	}
}
