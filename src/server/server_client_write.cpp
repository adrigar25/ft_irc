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

        // if there's nothing to send, just clear POLLOUT and return
        while (offset < out.size()) {
            ssize_t n = send(fd, out.data() + offset, out.size() - offset, 0);
            // if send succeeded in sending some bytes, update offset and check if done
            if (n > 0) {
                offset += static_cast<size_t>(n);
                if (offset >= out.size()) {
                    // all sent
                    out.clear();
                    offset = 0;
                    // stop listening for POLLOUT until new data arrives
                    this->fds[idx].events &= ~POLLOUT;
                    break;
                }
                // continue sending remaining bytes
                continue;
            }
                if (n == 0) {
                handleDisconnectionByIndex(idx);
                return true;
            }
            //if send returned -1, check errno
            // if interrupted, just retry
            if (errno == EINTR)
                continue;
            //if would block, just wait for next POLLOUT event; the remaining data is still in the buffer
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            // fatal error
            std::cerr << "send failed on fd " << fd << ": " << strerror(errno) << std::endl;
            handleDisconnectionByIndex(idx);
            return true;
        }
        return false;
    }
    return false;
}
