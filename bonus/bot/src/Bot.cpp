

#include "Bot.hpp"
#include "BotOptions.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

Bot::Bot(const std::string &host, const int port, const std::string &password, const std::string &nick)
	: host(host), port(port), password(password), nick(nick), irc(NULL), cm(NULL), rh(NULL)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	this->irc = new IRCConnection();
	this->cm = new ChannelManager(this->irc);
	this->rh = new RequestHandler(this->irc, this->cm, this->nick);
	std::cout << "Bot initialized with nick: " << this->nick << ", host: " << this->host << ", port: " << this->port << "\n";
}

Bot::Bot(const Bot &other)
	: host(other.host), port(other.port), password(other.password), nick(other.nick), irc(NULL), cm(NULL), rh(NULL)
{
	if (other.irc)
		this->irc = new IRCConnection(*other.irc);
	if (other.cm)
		this->cm = new ChannelManager(*other.cm);
	if (other.rh)
		this->rh = new RequestHandler(*other.rh);
}

Bot &Bot::operator=(const Bot &other)
{
	if (this != &other) {
		this->host = other.host;
		this->port = other.port;
		this->password = other.password;
		this->nick = other.nick;

		if (this->irc)
			delete this->irc;
		if (this->cm)
			delete this->cm;
		if (this->rh)
			delete this->rh;

		this->irc = other.irc ? new IRCConnection(*other.irc) : NULL;
		this->cm = other.cm ? new ChannelManager(*other.cm) : NULL;
		this->rh = other.rh ? new RequestHandler(*other.rh) : NULL;
	}
	return *this;
}

Bot::~Bot()
{
	if (this->rh)
		delete (this->rh);
	if (this->cm)
		delete (this->cm);
	if (this->irc)
		delete (this->irc);
}

std::string Bot::getNick() const
{
	return nick;
}

std::string Bot::getHost() const
{
	return host;
}

std::string Bot::getPassword() const
{
	return password;
}

void Bot::setPort(int p)
{
	if (p > 0)
		this->port = p;
}

int Bot::connectToServer()
{
	if (!this->irc)
		return -1;
	return this->irc->connectTo(this->host, this->port);
}

void Bot::run()
{
	if (!this->irc || !this->irc->isConnected())
	{
		std::cerr << "Bot: socket not connected\n";
		return;
	}

	this->irc->sendRaw("PASS " + this->password);
	this->irc->sendRaw("NICK " + this->nick);
	this->irc->sendRaw("USER " + this->nick + " 0 * :" + this->nick);

	while (true)
	{
		std::string line = this->irc->recvLine();
		if (line.empty())
		{
			std::cerr << "Conexión cerrada\n";
			break;
		}
		std::string tline = line.substr(0, line.size() - 2);
		std::cout << "< " << tline << "\n";

		if (tline.rfind("PING", 0) == 0)
		{
			std::cout << "Responding to PING\n";
			std::string token = tline.substr(5);
			if (!token.empty() && token[0] == ':')
				token = token.substr(1);
			this->irc->sendRaw("PONG :" + token);
			continue;
		}

		if (this->rh)
			this->rh->handleLine(line);
	}
}
