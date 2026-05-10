#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>

void Server::handleEvents()
{
    ssize_t n = 0;

    while (true)
    {
        int nfds = this->fds.size();
        if (nfds == 0) {
            // nothing to poll
            continue;
        }

        int ready = poll(this->fds.data(), nfds, -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            std::cerr << "poll error: " << strerror(errno) << std::endl;
            break;
        }

        if (this->fds[0].revents & POLLIN)
            handleNewConnection();

        for (int i = 1; i < nfds; i++)
        {
            if (this->fds[i].revents & POLLIN)
            {
                int fd = this->fds[i].fd;
                char buffer[1024];
                n = recv(fd, buffer, sizeof(buffer) - 1, 0);

                if (n <= 0)
                {
                    handleDisconnection(i);
                    i--;
                    nfds = this->fds.size();
                    continue;
                }
                buffer[n] = '\0';
                std::string &acc = this->buffers[fd];
                acc.append(buffer, n);
                size_t pos;
                while ((pos = acc.find('\n')) != std::string::npos)
                {
                    std::string line = acc.substr(0, pos);
                    if (!line.empty() && line.back() == '\r')
                        line.erase(line.size() - 1);
                    std::cout << "Received line from fd " << fd << ": " << line << std::endl;
                    handleClientMessage(fd, line);
                    acc.erase(0, pos + 1);
                }
            }

            if (this->fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                handleDisconnection(i);
                i--;
                nfds = this->fds.size();
                continue;
            }
        }
    }
}
