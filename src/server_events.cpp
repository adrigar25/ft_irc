#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>

int Server::performPoll()
{
    int nfds = this->fds.size();
    int ready = poll(this->fds.data(), nfds, -1);
    if (ready < 0) {
        if (errno == EINTR) return 0;
        std::cerr << "poll error: " << strerror(errno) << std::endl;
        return -1;
    }
    return ready;
}



bool Server::handleClientRead(int idx)
{
    int fd = -1;
    char buffer[1024];

    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (!(this->fds[idx].revents & POLLIN))
        return false;
    fd = this->fds[idx].fd;
    ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (n <= 0)
    {
        handleDisconnection(idx);
        return true;
    }
    buffer[n] = '\0';
    std::string &acc = this->buffers[fd];
    acc.append(buffer, n);
    processClientBuffer(fd);
    return false;
}

void Server::processClientBuffer(int fd)
{
    std::string &acc = this->buffers[fd];
    size_t pos;
    while ((pos = acc.find('\n')) != std::string::npos)
    {
        std::string line = acc.substr(0, pos);
        if (!line.empty() && line.back() == '\r')
            line.erase(line.size() - 1);
        std::cout << "Received line from fd " << fd << ": " << line << std::endl;
        try {
            handleClientMessage(fd, line);
        } catch (const std::exception &e) {
            std::cerr << "Client error fd " << fd << ": " << e.what() << std::endl;
            for (int i = 1; i < (int)this->fds.size(); ++i) {
                if (this->fds[i].fd == fd) { handleDisconnection(i); break; }
            }
        } catch (...) {
            std::cerr << "Unknown client error fd " << fd << std::endl;
            for (int i = 1; i < (int)this->fds.size(); ++i) {
                if (this->fds[i].fd == fd) { handleDisconnection(i); break; }
            }
        }
        acc.erase(0, pos + 1);
    }
}

bool Server::handleClientErrorEvents(int idx)
{
    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (this->fds[idx].revents & (POLLHUP | POLLERR | POLLNVAL))
    {
        handleDisconnection(idx);
        return true;
    }
    return false;
}

void Server::processClientEvents()
{
    for (int i = 1; i < (int)this->fds.size(); ++i)
    {
        if (handleClientRead(i)) { i--; continue; }
        if (handleClientErrorEvents(i)) { i--; continue; }
    }
}

void Server::handleEvents()
{
    int ready = 0;
    while (true)
    {
        if (this->fds.size() == 0)
            continue;
        ready = performPoll();
        if (ready == 0)
            continue;
        if (ready < 0)
            break;
        if (!this->fds.empty() && (this->fds[0].revents & POLLIN))
        {
            try {
                handleNewConnection();
            } catch (const std::exception &e) {
                std::cerr << "Accept error: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown accept error" << std::endl;
            }
        }

        try {
            processClientEvents();
        } catch (const std::exception &e) {
            std::cerr << "Error processing client events: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error processing client events" << std::endl;
        }
    }
}
