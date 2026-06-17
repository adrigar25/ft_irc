/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   RequestHandler.cpp								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:09:03 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 17:06:12 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "IrcConnection.hpp"
#include "ChannelManager.hpp"
#include "BotOptions.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>


static std::string cleanIrcParam(const std::string &s)
{
	std::string value = s;
	if (!value.empty() && value[0] == ':')
		value.erase(0, 1);
	return (value);
}

RequestHandler::RequestHandler(IRCConnection *irc, ChannelManager *cm, const std::string &nick)
	: irc(irc), cm(cm), nick(nick) {}

RequestHandler::~RequestHandler() {}

const BotCmd &RequestHandler::chooseReply(const std::string &text) const
{
	std::string cmd = text.substr(1);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	for (size_t i = 0; BOT_COMMANDS[i].trigger != ""; ++i)
	{
		if (cmd == BOT_COMMANDS[i].trigger)
			return (BOT_COMMANDS[i]);
	}
	return (BOT_COMMANDS[0]);
}

static Message parseMessage(const std::string &line)
{
	Message m;

	std::string raw = line.substr(0, line.find("\r\n"));

	std::string rest = raw;

	// prefix
	if (!rest.empty() && rest[0] == ':')
	{
		size_t sp = rest.find(' ');
		std::string prefix = rest.substr(1, sp - 1);

		size_t bang = prefix.find('!');
		if (bang != std::string::npos)
			m.user = prefix.substr(0, bang);

		rest = rest.substr(sp + 1);
	}

	std::istringstream iss(rest);
	iss >> m.cmd;

	std::string param;

	while (iss >> param)
	{
		if (param[0] == ':')
		{
			m.trailing = param.substr(1);
			std::string extra;
			std::getline(iss, extra);
			if (!extra.empty())
				m.trailing += extra;
			break;
		}
		m.params.push_back(param);
	}

	return m;
}


static std::string getCommandList()
{
	std::string list;
	for (size_t i = 1; BOT_COMMANDS[i].trigger != ""; ++i)
	{
		list += "!" + BOT_COMMANDS[i].trigger + " ";
	}
	return (list);
}

static void handleKick(ChannelManager *cm, const std::string &channel, const std::string &kickedUser, const std::string &botNick)
{

	if (kickedUser == botNick && cm->isInChannel(channel))
		cm->deleteChannel(channel);
}

void RequestHandler::handlePRIVMSG(
	const std::string &user,
	const std::string &channel,
	const std::string &msg)
{
	if (msg.empty() || msg[0] != '!')
		return;

	std::istringstream iss(msg);
	std::string trigger;
	iss >> trigger;

	const BotCmd &reply = this->chooseReply(trigger);
	
	if (reply.trigger.empty())
		return;

	if (!this->cm->isInChannel(channel))
		return;

	if (reply.isOp && !this->cm->checkUserOP(channel, user))
	{
		this->irc->sendRaw("PRIVMSG " + channel + " :Not authorized.");
		return;
	}

	if (reply.action == "PRIVMSG")
	{
		std::string response = reply.response;

		if (reply.trigger == "help")
			response += getCommandList();

		else if (reply.trigger == "dice")
			response += std::to_string(rand() % 6 + 1);

		else if (reply.trigger == "coin")
			response += (rand() % 2 ? "Heads" : "Tails");

		this->irc->sendRaw("PRIVMSG " + channel + " :" + response);
		return;
	}

	if (reply.action == "KICK")
	{
		std::string targetUser;
		iss >> targetUser;
		this->irc->sendRaw("KICK " + channel + " " + targetUser + " :" + reply.response);
		return;
	}
}

static void handleInvite(ChannelManager *cm, const std::string &channel)
{
	cm->joinChannel(channel);
}

static void handleUserPartOrQuit(IRCConnection *irc, ChannelManager *cm, const std::vector<std::string> &params)
{
	if (params.size() < 1)
		return;

	std::string channel = cleanIrcParam(params[0]);

	if (cm->isOnlyUserInChannel(channel))
	{
		irc->sendRaw("PART " + channel + " :I am the only user left in this channel.");
		cm->deleteChannel(channel);
	}
}


static int getCommandType(const std::string &cmd)
{
	if (cmd == "PING")
		return CMD_PING;
	if (cmd == "INVITE")
		return CMD_INVITE;
	if (cmd == "KICK")
		return CMD_KICK;
	if (cmd == "PART")
		return CMD_PART;
	if (cmd == "QUIT")
		return CMD_QUIT;
	if (cmd == "PRIVMSG")
		return CMD_PRIVMSG;
	return CMD_UNKNOWN;
}

void RequestHandler::handleLine(const std::string &line)
{
	this->irc->isConnected();

	Message m = parseMessage(line);

	std::cout << "Parsed line: user=" << m.user
			  << ", cmd=" << m.cmd
			  << ", trailing=" << m.trailing << std::endl;
	std::cout << "Params: ";
	for (size_t i = 0; i < m.params.size(); ++i)
		std::cout << m.params[i] << " ";
	std::cout << std::endl;

	switch (getCommandType(m.cmd))
	{
	case CMD_PING:
		this->irc->sendRaw("PONG :" + m.trailing);
		break;

	case CMD_INVITE:
		handleInvite(this->cm, m.trailing);
		break;

	case CMD_KICK:
		handleKick(this->cm, m.params[0], m.params[1], this->nick);
		break;

	case CMD_PART:
	case CMD_QUIT:
		handleUserPartOrQuit(this->irc, this->cm, m.params);
		break;

	case CMD_PRIVMSG:
		if (m.params.size() >= 1)
			this->handlePRIVMSG(m.user, m.params[0], m.trailing);
		break;

	default:
		break;
	}
}
