#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>

bool Server::handlePART(User *user, const std::string &params)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    std::string channelsPart = params;
    std::string msgPart;
    size_t sp = params.find(' ');
    if (sp != std::string::npos) {
        channelsPart = params.substr(0, sp);
        msgPart = params.substr(sp + 1);
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

    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        const std::string &channelName = *it;
        if(this->channels.find(channelName) != this->channels.end())
        {
            user->leaveChannel(this->channels[channelName]);
        }
    }
    return true;
}
