#include "Server.hpp"
#include "socket_utils.h"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void Server::handleNewConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (newSocket < 0)
        throw IrcException(IRC_ERR_ACCEPTING_CONNECTION, std::string("accept failed: ") + strerror(errno));

    setSocketNonBlocking(newSocket);
    setSocketCloexec(newSocket);
    pushPollFd(newSocket, POLLIN);

    std::stringstream ss;
    ss << "Guest" << newSocket;
    User* newUser = new User(newSocket, ss.str());
    addUser(newUser);

    std::cout << "New connection accepted | socket: " << newUser->getSocket() << " | nickname: " << newUser->getNickname() << std::endl;
}

void Server::handleDisconnectionByIndex(int idx)
{
    if (idx <= 0 || idx >= (int)this->fds.size()) {
        std::cerr << "handleDisconnectionByIndex: invalid idx " << idx << std::endl;
        return;
    }

    int fd = this->fds[idx].fd;

    std::cout << "Client disconnected | fd: " << fd << std::endl;
    close(fd);
    this->deleteUser(fd);
    this->fds.erase(this->fds.begin() + idx);
}

void Server::handleDisconnectionByFd(int fd)
{
    for (size_t i = 1; i < this->fds.size(); ++i) {
        if (this->fds[i].fd == fd) {
            handleDisconnectionByIndex(static_cast<int>(i));
            return;
        }
    }
    std::cerr << "handleDisconnectionByFd: fd not found " << fd << std::endl;
}
