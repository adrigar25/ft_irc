
#include "User.hpp"

User::User(std::string name): name(name){};

User::~User(){};

void User::joinChannel(Channel *channel)
{
    this->currentChannel = channel;
}

std::string User::getNickname() const
{
    return this->name;
}