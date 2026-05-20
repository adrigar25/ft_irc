#include "Server.hpp"
#include "User.hpp"
#include <string>
#include <map>

bool Server::handleNICK(User *user, const std::string &nick)
{
    if (nick.empty()) {
        sendToUser(user, std::string("431 NICK :No nickname given"));
        return true;
    }
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it)
    {
        if (it->second != user && it->second->getNickname() == nick)
        {
            sendToUser(user, std::string("433 NICK :Nickname is already in use"));
            return true;
        }
    }
    user->setNickname(nick);
    sendToUser(user, std::string("Nickname set"));
    return true;
}
