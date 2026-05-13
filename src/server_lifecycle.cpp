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
    std::cout << "Server destroyed" << std::endl;
}
void Server::createServerSocket()
{
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0) {
        std::string msg = std::string("socket failed: ") + strerror(errno);
        std::cerr << msg << std::endl;
        throw errorStartingServerException();
    }
    if (!setSocketNonBlocking(this->serverSocket)) {
        close(this->serverSocket);
        this->serverSocket = -1;
        throw errorSettingNonblockingException();
    }
    if (!setSocketCloexec(this->serverSocket)) {
        close(this->serverSocket);
        this->serverSocket = -1;
        throw errorSettingCloexecException();
    }
    std::cout << "Socket created" << std::endl;
}

void Server::setSocketOptions()
{
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
    }
    std::cout << "Socket options set" << std::endl;
}

void Server::bindServerSocket()
{
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
}

void Server::listenServerSocket()
{
    if (listen(this->serverSocket, 64) < 0) {
        std::string msg = std::string("listen failed: ") + strerror(errno);
        std::cerr << msg << std::endl;
        throw errorStartingServerException();
    }
    std::cout << "Server is listening on port " << this->port << "..." << std::endl;
}

void Server::setupPollFds()
{
    this->fds.clear();
    struct pollfd pfd;
    pfd.fd = this->serverSocket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->fds.push_back(pfd);
}

void Server::startServer()
{
    createServerSocket();
    setSocketOptions();
    bindServerSocket();
    listenServerSocket();
    setupPollFds();

    handleEvents();

    this->fds.clear();
    struct pollfd pfd;
    pfd.fd = this->serverSocket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->fds.push_back(pfd);

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
