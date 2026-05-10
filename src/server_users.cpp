#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

void Server::addUser(std::unique_ptr<User> user)
{
    int fd = user->getSocket();
    if(this->users.find(fd) != this->users.end())
       throw userAlreadyExistsException();
    this->users.insert(std::make_pair(fd, std::move(user)));
}

void Server::removeUser(int fd)
{
    std::map<int, std::unique_ptr<User> >::iterator it = this->users.find(fd);

    if (it != this->users.end()) {
        User *user = it->second.get();
        for (std::map<std::string, std::unique_ptr<Channel> >::iterator cit = this->channels.begin(); cit != this->channels.end(); ++cit) {
            try {
                cit->second->removeUser(user);
            } catch (...) {
            }
        }
        this->users.erase(it);
    }
}

Channel* Server::getChannel(const std::string &name)
{
    std::map<std::string, std::unique_ptr<Channel> >::iterator it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second.get();
    return NULL;
}
