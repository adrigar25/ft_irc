#include "Server.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <sstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void Server::setSocketNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::string msg = std::string("fcntl F_GETFL: ") + strerror(errno);
        perror("fcntl F_GETFL");
        throw errorSettingNonblockingException(msg);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::string msg = std::string("fcntl F_SETFL: ") + strerror(errno);
        perror("fcntl F_SETFL");
        throw errorSettingNonblockingException(msg);
    }
}

void Server::setSocketCloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        throw errorSettingCloexecException(std::string("fcntl F_GETFD: ") + strerror(errno));
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
        throw errorSettingCloexecException(std::string("fcntl F_SETFD: ") + strerror(errno));
}

void Server::handleNewConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int newSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (newSocket < 0)
        throw errorAcceptingConnectionException(std::string("accept failed: ") + strerror(errno));

    setSocketNonBlocking(newSocket);
    setSocketCloexec(newSocket);
    pushPollFd(newSocket, POLLIN);

    std::stringstream ss;
    ss << "Guest" << newSocket;
    User* newUser = new User(newSocket, ss.str());
    addUser(newUser);

    std::cout << "New connection accepted | socket: " << newUser->getSocket() << " | nickname: " << newUser->getNickname() << std::endl;
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
