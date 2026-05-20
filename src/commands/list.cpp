#include "Server.hpp"
#include "User.hpp"
#include <string>
#include <sstream>

bool Server::handleLIST(User *user, const std::string &params)
{
    (void)params;
    std::string response = "Channels:\n";
    for (std::map<std::string, Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        std::ostringstream oss;
        oss << it->second->getUserCount();
        response += it->first + " (" + oss.str() + " users)\n";
    }
    sendToUser(user, response);
    return true;
}
