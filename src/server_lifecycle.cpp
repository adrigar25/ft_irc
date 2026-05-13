#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(unsigned int port, std::string password): port(port), password(password)
{
    this->serverSocket = -1;
    std::cout << "Server created" << std::endl;
}

Server::~Server()
{
    for (size_t i = 1; i < this->fds.size(); ++i)
    {
        if (this->fds[i].fd >= 0)
            close(this->fds[i].fd);
    }
    if (this->serverSocket >= 0)
        close(this->serverSocket);
    // delete remaining users
    for (std::map<int, User*>::iterator it = this->users.begin(); it != this->users.end(); ++it) {
        delete it->second;
    }
    this->users.clear();
    // delete remaining channels
    for (std::map<std::string, Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        delete it->second;
    }
    this->channels.clear();
    std::cout << "Server destroyed" << std::endl;
}
void Server::createServerSocket()
{
    // Create socket
    // AF_INET: IPv4, SOCK_STREAM: TCP
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0)
        throw errorStartingServerException(0, std::string("socket failed: ") + strerror(errno));
    setSocketNonBlocking(this->serverSocket);
    setSocketCloexec(this->serverSocket);
}
void Server::setSocketOptions()
{
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw errorStartingServerException(0, std::string("setsockopt failed: ") + strerror(errno));
    std::cout << "Socket options set" << std::endl;
}

void Server::bindServerSocket()
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw errorStartingServerException(0, std::string("bind failed: ") + strerror(errno));
    std::cout << "Bind successful" << std::endl;
}

void Server::listenServerSocket()
{
    if (listen(this->serverSocket, 64) < 0)
        throw errorStartingServerException(0, std::string("listen failed: ") + strerror(errno));
    std::cout << "Server is listening on port " << this->port << "..." << std::endl;
}

void Server::setupPollFds()
{
    this->fds.clear();
    pushPollFd(this->serverSocket, POLLIN);
}

void Server::pushPollFd(int fd, short events)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    this->fds.push_back(pfd);
}

void Server::startServer()
{
    try {
        createServerSocket();
        setSocketOptions();
        bindServerSocket();
        listenServerSocket();
        setupPollFds();
    }
    catch (const std::exception &e)
    {
        if(this->serverSocket >= 0) {
            close(this->serverSocket);
            this->serverSocket = -1;
        }
        std::cerr << "ERROR: " << e.what() << std::endl;
        return;
    }
    handleEvents();
}

void Server::stopServer()
{
    if (this->serverSocket >= 0) {
        close(this->serverSocket);
        this->serverSocket = -1;
    }
    std::cout << "Server stopped" << std::endl;
}
