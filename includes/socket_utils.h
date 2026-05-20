#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

void setSocketNonBlocking(int fd);
void setSocketCloexec(int fd);
std::string formatMessage(const std::string &message);

#endif
