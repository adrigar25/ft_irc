#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>
#include <sstream>

void Server::addUser(User* user)
{
    this->services.users().add(user);
}

void Server::deleteUser(int fd)
{
    User* user = this->services.users().findByFd(fd);
    if (!user) return;
    const std::map<std::string, Channel*>& channels = this->services.channels().getAll();
    for (std::map<std::string, Channel*>::const_iterator cit = channels.begin(); cit != channels.end(); ++cit) {
        try {
            if (cit->second) cit->second->deleteUser(user);
        } catch (const std::exception &e) {
            std::cerr << "ERROR: error removing user from channel " << cit->first << ": " << e.what() << std::endl;
        }
    }
    this->services.users().remove(fd);
}

User* Server::getUserByFd(int fd)
{
    return this->services.users().findByFd(fd);
}
