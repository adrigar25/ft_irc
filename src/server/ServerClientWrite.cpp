#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>

bool Server::handleClientWrite(int idx)
{
    if (idx < 0 || idx >= (int)this->fds.size())
        return false;
    if (this->fds[idx].revents & POLLOUT)
    {
        int fd = this->fds[idx].fd;
        User *user = this->services.users().findByFd(fd);
        if (!user) {
            handleDisconnectionByIndex(idx);
            return true;
        }

        std::string &out = user->getOutBuffer();
        size_t &offset = user->getOutOffset();

        while (offset < out.size()) {
            ssize_t n = send(fd, out.data() + offset, out.size() - offset, 0);
            if (n > 0) {
                offset += static_cast<size_t>(n);
                if (offset >= out.size()) {
                    out.clear();
                    offset = 0;
                    this->fds[idx].events &= ~POLLOUT;
                    break;
                }
                continue;
            }
                if (n == 0) {
                handleDisconnectionByIndex(idx);
                return true;
            }
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            std::cerr << "send failed on fd " << fd << ": " << strerror(errno) << std::endl;
            handleDisconnectionByIndex(idx);
            return true;
        }
        return false;
    }
    return false;
}
