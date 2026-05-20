#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>

bool Server::handleJOIN(User *user, const std::string &params)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = params;
    std::string keysPart;
    size_t sp = params.find(' ');
    if (sp != std::string::npos) {
        channelsPart = params.substr(0, sp);
        keysPart = params.substr(sp + 1);
    }

    std::istringstream ciss(channelsPart);
    std::string value;
    while (std::getline(ciss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            channelNames.push_back(value);
        else if (!value.empty())
            sendToUser(user, std::string("Invalid channel name: ") + value);
    }

    if (!keysPart.empty()) {
        std::istringstream kiss(keysPart);
        while (std::getline(kiss, value, ',')) {
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);
            if (!value.empty())
                keys.push_back(value);
        }
    }

    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        const std::string &channelName = *it;
        if(this->channels.find(channelName) == this->channels.end())
            createChannel(channelName, user);
        else
            user->joinChannel(this->channels[channelName]);
        
        // Notify channel members about the JOIN and send NAMES to the joining user
        Channel *channel = getChannel(channelName);
        if (channel) {
            char hostname[256];
            if (gethostname(hostname, sizeof(hostname)) != 0)
                std::strcpy(hostname, "localhost");

            std::string uname = user->getUsername();
            if (uname.empty()) uname = "~";

            // Broadcast JOIN to all members (including the joining user)
            std::string joinMsg = std::string(":") + user->getNickname() + "!" + uname + "@" + hostname + " JOIN " + channelName + "\r\n";
            sendToChannel(channel, joinMsg, NULL);

            // Build NAMES reply for the joining user
            const std::map<int, User*>& usersMap = channel->getUsers();
            std::string namesList;
            for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
                if (!namesList.empty()) namesList += " ";
                if (channel->isUserOperator(uit->second))
                    namesList += "@" + uit->second->getNickname();
                else
                    namesList += uit->second->getNickname();
            }
            std::string namesReply = std::string(":") + hostname + " 353 " + user->getNickname() + " = " + channelName + " :" + namesList + "\r\n";
            std::string endNames = std::string(":") + hostname + " 366 " + user->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
            sendToUser(user, namesReply);
            sendToUser(user, endNames);
        }
    }
    return true;
}
