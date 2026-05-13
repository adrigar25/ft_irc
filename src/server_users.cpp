#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

void Server::addUser(User* user)
{
    int fd = user->getSocket();
    if(this->users.find(fd) != this->users.end())
       throw userAlreadyExistsException(std::string("User already exists: fd ") + std::to_string(fd));
    this->users.insert(std::make_pair(fd, user));
}

void Server::removeUser(int fd)
{
    std::map<int, User*>::iterator it = this->users.find(fd);

    if (it != this->users.end()) {
        User *user = it->second;
        for (std::map<std::string, Channel*>::iterator cit = this->channels.begin(); cit != this->channels.end(); ++cit) {
            try {
                if (cit->second) cit->second->removeUser(user);
            } catch (const std::exception &e) {
                std::cerr << "ERROR: error removing user from channel " << cit->first << ": " << e.what() << std::endl;
            }
        }
        this->users.erase(it);
        delete user;
    }
}

/* getChannel moved to src/server_channels.cpp */

User* Server::getUserByNickname(const std::string &nickname)
{
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it) {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}