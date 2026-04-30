#include "Server.hpp"
#include <iostream>

Server::Server(unsigned int port, std::string password): port(port), password(password)
{
    std::cout << "Server created" << std::endl;
    startServer();
};

Server::~Server(){};

void Server::startServer()
{
    std::cout << "Server started at port " << this->port << std::endl;
}

void Server::stopServer()
{
    std::cout << "Server stopped" << std::endl;
}

void Server::addUser(User *user)
{
    if(this->users.find(user->getNickname()) != this->users.end())
       throw userAlreadyExistsException();
    this->users.insert(std::make_pair(user->getNickname(), user));
}

void Server::removeUser(User *user)
{
    if(this->users.find(user->getNickname()) == this->users.end())
        throw userNotFoundException();
    this->users.erase(user->getNickname());
}

void Server::createChannel(std::string name, User *creator)
{
    if (this->channels.find(name) != this->channels.end())
        throw channelAlreadyExistsException();
    Channel *c = new Channel(name, creator);
    this->channels.insert(std::make_pair(name, c));
}

Channel* Server::getChannel(const std::string &name)
{
    std::map<std::string, Channel*>::iterator it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}