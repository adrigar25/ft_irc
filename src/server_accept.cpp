#include "Server.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>  
#include <cerrno>
#include <unistd.h>

void Server::handleNewConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (newSocket < 0)
        throw errorAcceptingConnectionException();

    struct pollfd pfd;
    pfd.fd = newSocket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->fds.push_back(pfd);

    std::stringstream ss;
    ss << "Guest" << newSocket;
    std::unique_ptr<User> newUser(new User(newSocket, ss.str()));
    addUser(std::move(newUser));

    User *raw = this->users[newSocket].get();
    std::cout << "New connection accepted | socket: " << raw->getSocket() << " | nickname: " << raw->getNickname() << std::endl;
}

void Server::handleDisconnection(int idx)
{
    int nfds = this->fds.size();
    if (idx <= 0 || idx >= nfds) {
        std::cerr << "handleDisconnection: invalid idx " << idx << std::endl;
        return;
    }

    int fd = this->fds[idx].fd;

    std::cout << "Client disconnected | fd: " << fd << std::endl;

    close(fd);

    this->removeUser(fd);

    this->fds.erase(this->fds.begin() + idx);

    std::map<int, std::string>::iterator bit = this->buffers.find(fd);
    if (bit != this->buffers.end())
        this->buffers.erase(bit);
}
