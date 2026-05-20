#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleINVITE(User *user, const std::string &params)
{
    (void)params;
    sendToUser(user, std::string("INVITE command not implemented yet"));
    return true;
}
