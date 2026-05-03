
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

Server::Server(unsigned int port, std::string password): port(port), password(password)
{
    std::cout << "Server created" << std::endl;
};

Server::~Server(){};

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
}

void Server::stopServer()
{
    close(this->serverSocket);
    std::cout << "Server stopped" << std::endl;
}

void Server::addUser(User *user)
{
    if(this->users.find(user->getNickname()) != this->users.end())
       throw userAlreadyExistsException();
    this->users.insert(std::make_pair(user->getNickname(), user));
}

void Server::removeUser(User *user)
{
    if(this->users.find(user->getNickname()) == this->users.end())
        throw userNotFoundException();
    this->users.erase(user->getNickname());
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