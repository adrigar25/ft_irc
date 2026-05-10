#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>

void Server::sendToUser(User *user, const std::string &message)
{
    std::string msg = message;
    if (msg.find("\r\n") == std::string::npos)
        msg += "\r\n";

    const char *buf = msg.c_str();
    size_t total = 0;
    size_t len = msg.size();

    while (total < len)
    {
        ssize_t n = send(user->getSocket(), buf + total, len - total, 0);
        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::cerr << "send would block on fd " << user->getSocket() << std::endl;
                break;
            }
            std::cerr << "send failed: " << strerror(errno) << std::endl;
            break;
        }
        total += static_cast<size_t>(n);
    }
}
