#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include "Server.hpp"
#include "User.hpp"
#include <string>
#include <iostream>

bool Server::handlePRIVMSG(User *user, const std::string &params)
{
    size_t sp = params.find(' ');
    std::string target = (sp == std::string::npos) ? params : params.substr(0, sp);

    std::string msg;
    if (sp != std::string::npos) {
        msg = params.substr(sp + 1);
        if (!msg.empty() && msg[0] == ' ') msg.erase(0,1);
        if (!msg.empty() && msg[0] == ':') msg.erase(0,1);
        if (!msg.empty() && msg[msg.size() - 1] == '\r') msg.erase(msg.size() - 1, 1);
    } else {
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }
    if(target.empty() || msg.empty()) {
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }

    // Build a full prefix `:nick!user@host` to improve client parsing (WeeChat expects a full prefix)
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
        std::strcpy(hostname, "localhost");
    std::string uname = user->getUsername();
    if (uname.empty()) uname = "~";
    std::string out = std::string(":") + user->getNickname() + "!" + uname + "@" + hostname + " PRIVMSG " + target + " :" + msg + "\r\n";

    if(target.empty()) {
        sendToUser(user, std::string("411 PRIVMSG :No recipient given"));
        return true;
    }

    if(target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!') {
        Channel *channel = getChannel(target);
        if (!channel) {
            sendToUser(user, std::string("403 ") + target + " :No such channel");
            return true;
        }
        if(!channel->hasUser(user))
        {
            sendToUser(user, std::string("404 ") + target + " :Cannot send to channel");
            return true;
        }
        // Debug: log recipients for PRIVMSG
        std::cout << "[DEBUG] PRIVMSG from " << user->getNickname() << " to channel " << target << " (userCount=" << channel->getUserCount() << ")" << std::endl;
        const std::map<int, User*>& usersMap = channel->getUsers();
        for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
            if (uit->second && uit->second != user)
                std::cout << "[DEBUG]  will send to " << uit->second->getNickname() << " fd=" << uit->first << std::endl;
        }
        sendToChannel(channel, out, user);
    } else {
        User *dest = this->getUserByNickname(target);
        if (!dest) {
            sendToUser(user, std::string("401 ") + target + " :No such nick/channel");
            return true;
        }
        sendToUser(dest, out);
    }
    return true;
}
