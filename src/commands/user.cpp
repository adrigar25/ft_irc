#include "Server.hpp"
#include "User.hpp"
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstring>

bool Server::handleUSER(User *user, const std::string &params)
{
    if (params.empty()) {
        sendToUser(user, std::string("461 USER :Not enough parameters"));
        return true;
    }
    std::istringstream iss(params);
    std::string username, mode, unused, real;
    iss >> username >> mode >> unused;
    std::getline(iss, real);
    if (!real.empty() && real[0] == ' ') real.erase(0,1);
    if (!real.empty() && real[0] == ':') real.erase(0,1);

    user->setUsername(username);
    sendToUser(user, std::string("Username set"));

    if (user->isPassSet() && user->isNickSet() && user->isUserSet()) {
        user->setAuthenticated(true);
        /* Send RPL_WELCOME (001) so IRC clients treat the connection as
         * registered. Use the local hostname as server name; fall back to
         * "localhost" on error. */
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) != 0)
            std::strcpy(hostname, "localhost");
        std::string welcome = std::string(":") + hostname + " 001 " + user->getNickname() + " :Welcome to " + hostname;
        sendToUser(user, welcome);
    }
    return true;
}
