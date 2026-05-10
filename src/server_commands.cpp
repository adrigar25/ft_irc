#include "Server.hpp"
#include "User.hpp"
#include <iostream>
#include <sstream>

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

void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{
    int cmd = -1;
    if (command == "PASS") cmd = 1;
    else if (command == "NICK") cmd = 2;
    else if (command == "USER") cmd = 3;
    
    std::cout << "Executing command: " << command << " with args: " << args << " for user: " << user->getNickname() << std::endl;
    if (!user->isAuthenticated())
    {

        switch (cmd)
        {
            case 1: // PASS
            {
                if (args.empty())
                {
                    sendToUser(user, std::string("461 PASS :Not enough parameters"));
                    return;
                }
                if (args == this->password)
                {
                    user->setPass(true);
                    sendToUser(user, std::string("Password accepted"));
                    return;
                }
                sendToUser(user, std::string("Invalid password"));
                return;
            }
            case 2: // NICK
            {
                if (args.empty())
                {
                    sendToUser(user, std::string("431 NICK :No nickname given"));
                    return;
                }
                for (std::map<int, std::unique_ptr<User> >::iterator it = this->users.begin(); it != this->users.end(); ++it)
                {
                    if (it->second.get() != user && it->second->getNickname() == args)
                    {
                        sendToUser(user, std::string("433 NICK :Nickname is already in use"));
                        return;
                    }
                }
                user->setNickname(args);
                sendToUser(user, std::string("Nickname set"));
                return;
            }
            case 3: // USER
            {
                if (args.empty()) {
                    sendToUser(user, std::string("461 USER :Not enough parameters"));
                    return;
                }
                std::istringstream iss(args);
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
                return;
            }
            default:
            {
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
        }
    }

    // Authenticated commands would go here
}
