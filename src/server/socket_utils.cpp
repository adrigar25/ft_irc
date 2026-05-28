#include "socket_utils.h"
#include <fcntl.h>
#include <stdexcept>
#include <cstring>
#include <errno.h>

void setSocketNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error(std::string("fcntl F_GETFL: ") + strerror(errno));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error(std::string("fcntl F_SETFL: ") + strerror(errno));
    }
}

void setSocketCloexec(int fd)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        throw std::runtime_error(std::string("fcntl F_GETFD: ") + strerror(errno));
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
        throw std::runtime_error(std::string("fcntl F_SETFD: ") + strerror(errno));
}

std::string formatMessage(const std::string &message)
{
    std::string msg = message;
    if (msg.find("\r\n") == std::string::npos)
        msg += "\r\n";
    return msg;
}
