#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleMODE(User *user, const std::string &params)
{
    (void)params;
    sendToUser(user, std::string("MODE command not implemented yet"));
    return true;
}
