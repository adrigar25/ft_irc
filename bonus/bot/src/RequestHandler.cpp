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
static void parseMessage(const std::string &line, std::string &prefix, std::string &cmd, std::vector<std::string> &params)
{
	std::string raw = line.substr(0, line.find("\r\n"));
	if (raw.empty())
		return;
	std::string rest = raw;
	if (rest[0] == ':')
	{
		size_t sp = rest.find(' ');
		if (sp == std::string::npos)
			return;
		prefix = rest.substr(1, sp - 1);
		rest = rest.substr(sp + 1);
	}
	std::istringstream iss(rest);
	iss >> cmd;
	std::string param;
	while (iss >> param)
		params.push_back(param);
}

static void handlePRIVMSG(IRCConnection *irc, const std::string &target, const BotCmd &reply, const std::vector<std::string> &params)
{

	if (reply.action == "PRIVMSG")
	{
		std::string response = reply.response;
		if (reply.trigger == "help")
		{
			for (size_t i = 1; BOT_COMMANDS[i].trigger != ""; ++i)
				response += "!" + BOT_COMMANDS[i].trigger + " ";
			irc->sendRaw("PRIVMSG " + target + " :" + response);
		}
		else if (reply.trigger == "dice")
		{
			int roll = rand() % 6 + 1;
			response += std::to_string(roll);
			irc->sendRaw("PRIVMSG " + target + " :" + response);
		}
		else if (reply.trigger == "coin")
		{
			std::string coin = (rand() % 2 == 0) ? "Heads" : "Tails";
			response += coin;
			irc->sendRaw("PRIVMSG " + target + " :" + response);
		}
		else
			irc->sendRaw("PRIVMSG " + target + " :" + reply.response);
	}

	if (reply.action == "KICK")
	{
		if (params.size() < 3)
			return;
		irc->sendRaw("KICK " + target + " " + params[2] + " :" + reply.response);
	}
}

void RequestHandler::handleLine(const std::string &line)
{
	this->irc->isConnected();
	std::string prefix, cmd;
	std::vector<std::string> params;
	parseMessage(line, prefix, cmd, params);

	if (cmd == "INVITE")
	{
		this->cm->joinChannel(cleanIrcParam(params[1]));
		return;
	}

	if (cmd == "PRIVMSG")
	{
		if (params.size() < 2)
			return;

		std::string target = cleanIrcParam(params[0]);
		std::string message = cleanIrcParam(params[1]);

		if (message.empty() || message[0] != '!')
			return;

		const BotCmd &reply = chooseReply(message);

		if (reply.trigger.empty())
			return;

		if (!this->cm->isInChannel(target))
			return;

		if(reply.isOp && this->cm->checkUserOP(target, prefix.substr(0, prefix.find('!'))) == false)
		{
			this->irc->sendRaw("PRIVMSG " + target + " :Not authorized.");
			return;
		}

		handlePRIVMSG(this->irc, target, reply, params);
		return;
	}

	if (cmd == "KICK")
	{
		if (params.size() < 2)
			return;

		std::string channel = cleanIrcParam(params[0]);
		std::string kickedUser = cleanIrcParam(params[1]);

		if (kickedUser == this->nick && this->cm->isInChannel(channel))
			this->cm->deleteChannel(channel);
	}

	if (cmd == "PART" || cmd == "QUIT")
	{
		if (params.size() < 1)
			return;

		std::string channel = cleanIrcParam(params[0]);
		std::string user = prefix.substr(0, prefix.find('!'));

		if (this->cm->isInChannel(channel) && user != this->nick)
		{
			this->irc->sendRaw("NAMES " + channel);
			std::string response = this->irc->recvLine();
			if (response.find(":" + this->nick) == std::string::npos)
				this->cm->deleteChannel(channel);
		}
	}
	/*if(si recibe un PART o un QUIT de quien sea que envíe un comando NAMES a ese canal y mire que no esté solo el)*/
}
