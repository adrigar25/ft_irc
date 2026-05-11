#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <utility>

void Server::handleClientCommand(User *user, const std::string &command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    std::string args;
    std::getline(iss, args);
    if (!args.empty() && args[0] == ' ')
        args.erase(0, 1);
    executeCommand(user, cmd, args);
}

void Server::handleClientMessage(int fd, const std::string &message)
{
    std::map<int, std::unique_ptr<User> >::iterator it = this->users.find(fd);
    if (it != this->users.end()) {
        User *user = it->second.get();
        handleClientCommand(user, message);
    } else {
        std::cerr << "handleClientMessage: user not found for fd " << fd << std::endl;
    }
}

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

bool Server::handleNICK(User *user, const std::string &nick)
{
    if (nick.empty()) {
        sendToUser(user, std::string("431 NICK :No nickname given"));
        return true;
    }
    for (std::map<int, std::unique_ptr<User> >::iterator it = this->users.begin(); it != this->users.end(); ++it)
    {
        if (it->second.get() != user && it->second->getNickname() == nick)
        {
            sendToUser(user, std::string("433 NICK :Nickname is already in use"));
            return true;
        }
    }
    user->setNickname(nick);
    sendToUser(user, std::string("Nickname set"));
    return true;
}

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
        sendToUser(user, std::string("Authentication successful"));
    }
    return true;
}

bool Server::handleQUIT(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("Goodbye!"));
    handleDisconnection(-1);
    return true;
}

bool Server::handleLIST(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    std::string response = "Channels:\n";
    for (std::map<std::string, std::unique_ptr<Channel> >::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        response += it->first + " (" + std::to_string(it->second->getUserCount()) + " users)\n";
    sendToUser(user, response);
    return true;
}

bool Server::handleINVITE(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("INVITE command not implemented yet"));
    return true;
}

bool Server::handleKICK(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("KICK command not implemented yet"));
    return true;
}

bool Server::handleMODE(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("MODE command not implemented yet"));
    return true;
}

void Server::createChannel(std::string name, User *creator)
{
    if (this->channels.find(name) != this->channels.end())
        throw channelAlreadyExistsException();

    std::unique_ptr<Channel> ch(new Channel(name, creator));
    this->channels.insert(std::make_pair(name, std::move(ch)));
}

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
        // Keep channel prefix (#,&,+,!) so map keys match incoming targets
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

    for (const auto &channelName : channelNames)
    {
        if(this->channels.find(channelName) == this->channels.end())
            createChannel(channelName, user);
        else
            user->joinChannel(this->channels[channelName].get());
    }
    return true;
}

bool Server::handlePART(User *user, const std::string &params)
{
    (void)params; // Unused parameter
    sendToUser(user, std::string("PART command not implemented yet"));
    return true;
}

bool Server::handlePRIVMSG(User *user, const std::string &params)
{
    std::list<User *> users;
    size_t sp = params.find(' ');
    std::string target = (sp == std::string::npos) ? params : params.substr(0, sp);

    std::string msg;
    if (sp != std::string::npos) {
        msg = params.substr(sp + 1);            // -> ":HOLA" (posible espacio inicial)
        if (!msg.empty() && msg[0] == ' ') msg.erase(0,1);
        if (!msg.empty() && msg[0] == ':') msg.erase(0,1); // quita la ':' sintáctica
        if (!msg.empty() && msg.back() == '\r') msg.pop_back(); // limpia CR si existe
    } else {
        // manejar error: faltan parámetros
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }
    if(target.empty() || msg.empty()) {
        sendToUser(user, std::string("461 PRIVMSG :Not enough parameters"));
        return true;
    }                   
    if(target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!') {
        Channel *ch = getChannel(target);
        if(!ch) {
            sendToUser(user, std::string("403 ") + target + " :No such channel");
            return true;
        }
        const std::map<int, User*>& usersMap = ch->getUsers();
        for (std::map<int, User*>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
            users.push_back(it->second);
    } else {
        User *targetUser = getUserByNickname(target);
        if (!targetUser) {
            sendToUser(user, std::string("401 ") + target + " :No such nick");
            return true;
        }
        users.push_back(targetUser);
    }
    std::string out = ":" + user->getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
    for (const auto &recipient : users) {
        sendToUser(recipient, out);
    }
    return true;
}

bool Server::handleUnknownCommand(User *user, const std::string &command)
{
    sendToUser(user, std::string("421 ") + command + " :Unknown command");
    return true;
}

void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{
    std::cout << "Executing command: " << command << " with args: " << args << " for user: " << user->getNickname() << std::endl;
    if (!user->isAuthenticated())
    {
        if (command == "PASS") { handlePASS(user, args); return; }
        if (command == "NICK") { handleNICK(user, args); return; }
        if (command == "USER") { handleUSER(user, args); return; }

        if (user->isPassSet() && user->isNickSet() && user->isUserSet())
        {
            user->setAuthenticated(true);
            sendToUser(user, std::string("Authentication successful"));
            return;
        }
        else
        {
            sendToUser(user, std::string("You're not authenticated"));
            return;
        }
    }

    if(command == "JOIN") { handleJOIN(user, args); return;}
    if(command == "PART") { handlePART(user, args); return;}
    if(command == "PRIVMSG") { handlePRIVMSG(user, args); return;}
    if(command == "QUIT") { handleQUIT(user, args); return;}
    if(command == "LIST") { handleLIST(user, args); return;}
    if(command == "INVITE") { handleINVITE(user, args); return;}
    if(command == "KICK") { handleKICK(user, args); return;}
    if(command == "MODE") { handleMODE(user, args); return;}


    handleUnknownCommand(user, command);
}
