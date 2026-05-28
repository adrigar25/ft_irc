#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleUnknownCommand(User *user, const std::string &command)
{
    sendToUser(user, std::string("421 ") + command + " :Unknown command");
    return true;
}
