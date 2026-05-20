#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleQUIT(User *user, const std::string &params)
{
    (void)params;
    sendToUser(user, std::string("Goodbye!"));
    return true;
}
