#include "User.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <poll.h>

Server::Server(unsigned int port, std::string password): port(port), password(password), userNum(0)
{
    std::cout << "Server created" << std::endl;
};

Server::~Server(){};

void Server::handleNewConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (newSocket < 0)
        throw errorAcceptingConnectionException();

    int idx = this->userNum + 1;
    this->fds[idx].fd = newSocket;
    this->fds[idx].events = POLLIN;
    this->userNum++;

    std::string nickname = "Guest" + std::to_string(newSocket);
    User *newUser = new User(newSocket, nickname);
    addUser(newUser);

    std::cout << "New connection accepted | socket: " << newUser->getSocket() << " | nickname: " << newUser->getNickname() << std::endl;

}

void Server::handleDisconnection(int idx)
{
    int nfds = this->userNum + 1;
    if (idx <= 0 || idx >= nfds) {
        std::cerr << "handleDisconnection: invalid idx " << idx << std::endl;
        return;
    }

    int fd = this->fds[idx].fd;

    std::cout << "Client disconnected | fd: " << fd << std::endl;

    close(fd);

    std::map<int, User*>::iterator it = this->users.find(fd);
    if (it != this->users.end()) {
        User *user = it->second;
        for (std::map<std::string, Channel*>::iterator cit = this->channels.begin(); cit != this->channels.end(); ++cit) {
            try {
                cit->second->removeUser(user);
            } catch (...) {
            }
        }
        delete user;
        this->users.erase(it);
    }

    this->fds[idx] = this->fds[this->userNum];
    this->fds[this->userNum].fd = -1;
    this->fds[this->userNum].events = 0;
    this->fds[this->userNum].revents = 0;
    this->userNum--;

    // Remove any partial buffer for this fd
    std::map<int, std::string>::iterator bit = this->buffers.find(fd);
    if (bit != this->buffers.end())
        this->buffers.erase(bit);
}

void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{
    std::cout << "Executing command: " << command << " with args: " << args << " for user: " << user->getNickname() << std::endl;
    if(!user->isAuthenticated())
    {

        if(!user->isPassSet() && command == "PASS")
        {
            if(args == this->password)
            {
                if(args.empty())
                {
                    sendToUser(user, std::string("461 PASS :Not enough parameters\r\n"));
                    return;
                }
                user->setPass(true);
                sendToUser(user, std::string("Password accepted\r\n"));
                return;

            }
            sendToUser(user, std::string("Invalid password\r\n"));
        }

        if(!user->isNickSet() && command == "NICK")
        {
            if(args.empty())
            {
                sendToUser(user, std::string("431 NICK :No nickname given\r\n"));
                return;
            }
            user->setNickname(args);
            sendToUser(user, std::string("Nickname set\r\n"));
            return;
        }

        if(!user->isUserSet() && command == "USER")
        {
            // Expected format: USER <username> <mode> <unused> :<realname>
            if (args.empty()) {
                sendToUser(user, std::string("461 USER :Not enough parameters\r\n"));
                return;
            }
            // Extract username (first token)
            size_t sp = args.find(' ');
            std::string uname = (sp == std::string::npos) ? args : args.substr(0, sp);
            if (!uname.empty() && uname[0] == ':')
                uname = uname.substr(1);

            user->setUsername(uname);
            sendToUser(user, std::string("Username set\r\n"));

            // If NICK and PASS already provided, complete registration now
            if (user->isPassSet() && user->isNickSet() && user->isUserSet()) {
                user->setAuthenticated(true);
                sendToUser(user, std::string("Authentication successful\r\n"));
            }
            return;
        }

        if(user->isPassSet() && user->isNickSet() && user->isUserSet())
        {
            user->setAuthenticated(true);
            sendToUser(user, std::string("Authentication successful\r\n"));
            return;
        }else
        {
            sendToUser(user, std::string("You're not authenticated\r\n"));
            return;
        }

    }
}

void Server::handleClientCommand(User *user, const std::string &command)
{
    std::string cmd = command.substr(0, command.find(' '));
    std::string args = command.substr(command.find(' ') + 1);
    executeCommand(user, cmd, args);
}

void Server::handleClientMessage(int fd, const std::string &message)
{
    std::map<int, User*>::iterator it = this->users.find(fd);
    if (it != this->users.end()) {
        User *user = it->second;
        handleClientCommand(user, message);
    } else {
        std::cerr << "handleClientMessage: user not found for fd " << fd << std::endl;
    }
}

void Server::handleEvents()
{
    this->fds[0].fd = this->serverSocket;
    this->fds[0].events = POLLIN;
    ssize_t n = 0;

    while (true)
    {
        int nfds = this->userNum + 1;
        poll(this->fds, nfds, -1);

        if (this->fds[0].revents & POLLIN)
            handleNewConnection();

        for (int i = 1; i < nfds; i++)
        {
            if (this->fds[i].revents & POLLIN)
            {
                int fd = this->fds[i].fd;
                char buffer[1024];
                n = recv(fd, buffer, sizeof(buffer) - 1, 0);

                if (n <= 0)
                {
                    handleDisconnection(i);
                    i--;
                    continue;
                }
                buffer[n] = '\0';
                std::string &acc = this->buffers[fd];
                acc += buffer;
                    size_t pos;
                    while ((pos = acc.find('\n')) != std::string::npos)
                {
                    std::string line = acc.substr(0, pos);
                        // remove trailing CR if present (handle CRLF or bare LF)
                        if (!line.empty() && line.back() == '\r')
                            line.erase(line.size() - 1);
                    std::cout << "Received line from fd " << fd << ": " << line << std::endl;
                    handleClientMessage(fd, line);
                    acc.erase(0, pos + 2);
                }
            }

            if (this->fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                handleDisconnection(i);
                i--;
                continue;
            }
        }
    }
}



void Server::startServer()
{
    std::cout.setf(std::ios::unitbuf);

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0) {
            std::string msg = std::string("socket failed: ") + strerror(errno);
            std::cerr << msg << std::endl;
        throw errorStartingServerException();
    }
    std::cout << "Socket created" << std::endl;

    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
    }

    std::cout << "Socket options set" << std::endl;

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::string msg = std::string("bind failed: ") + strerror(errno);
        std::cerr << msg << std::endl;
        throw errorStartingServerException();
    }

    std::cout << "Bind successful" << std::endl;

    if (listen(this->serverSocket, 3) < 0) {
        std::string msg = std::string("listen failed: ") + strerror(errno);
        std::cerr << msg << std::endl;
        throw errorStartingServerException();
    }

    std::cout << "Server is listening on port " << this->port << "..." << std::endl;
    handleEvents();
}

void Server::stopServer()
{
    close(this->serverSocket);
    std::cout << "Server stopped" << std::endl;
}

void Server::addUser(User *user)
{
    if(this->users.find(user->getSocket()) != this->users.end())
       throw userAlreadyExistsException();
    this->users.insert(std::make_pair(user->getSocket(), user));
}

void Server::removeUser(User *user)
{
    std::map<int, User*>::iterator it = this->users.find(user->getSocket());

    if (it != this->users.end()) {
        delete it->second;
        this->users.erase(it);
    }
}


Channel* Server::getChannel(const std::string &name)
{
    std::map<std::string, Channel*>::iterator it = this->channels.find(name);
    if (it != this->channels.end())
        return it->second;
    return NULL;
}

int Server::getPort() const
{
    return this->port;
}

int  Server::getServerSocket() const
{
    return this->serverSocket;
}

void Server::sendToUser(User *user, const std::string &message)
{
    std::string msg = message;
    if (msg.find("\r\n") == std::string::npos)
        msg += "\r\n";
    ssize_t n = send(user->getSocket(), msg.c_str(), msg.size(), 0);
    if (n < 0)
        std::cerr << "send failed: " << strerror(errno) << std::endl;
}