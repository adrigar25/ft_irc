#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleKICK(User *user, const std::string &params)
{
    (void)params;
    sendToUser(user, std::string("KICK command not implemented yet"));
    return true;
}
