#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handlePASS(User *user, const std::string &params)
{
    if (params.empty()) {
        sendToUser(user, std::string("461 PASS :Not enough parameters"));
        return true;
    }
    if (params == this->password) {
        user->setPass(true);
        sendToUser(user, std::string("Password accepted"));
        return true;
    }
    sendToUser(user, std::string("Invalid password"));
    return true;
}
