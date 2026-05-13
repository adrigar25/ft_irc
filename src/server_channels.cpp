#include "Server.hpp"
#include <iostream>

void Server::createChannel(std::string name, User *creator)
{
    if (this->channels.find(name) != this->channels.end())
        throw channelAlreadyExistsException(std::string("Channel already exists: ") + name);

    Channel *ch = new Channel(name, creator);
    this->channels.insert(std::make_pair(name, ch));
}

Channel* Server::getChannel(const std::string &name)
{
    std::map<std::string, Channel*>::iterator it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}
