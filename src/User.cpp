
#include "User.hpp"

User::User(int socket, const std::string &name): socket(socket), nickname(name), currentChannel(NULL) {}

User::~User() {}

void User::joinChannel(Channel *channel)
{
    this->currentChannel = channel;
}

std::string User::getNickname() const
{
    return this->nickname;
}