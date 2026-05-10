
#include "User.hpp"
#include "Channel.hpp"

User::User(int socket, const std::string &name)
    : socket(socket), nickname(name), username(""), currentChannel(NULL),
      nickSet(false), userSet(false), passGiven(false), authenticated(false) {}

User::~User() {}

std::string User::getNickname() const
{
    return this->nickname;
}

int User::getSocket() const
{
    return this->socket;
}

void User::setNickname(const std::string &name)
{
    this->nickname = name;
    this->nickSet = true;
}

void User::setUsername(const std::string &name)
{
    this->username = name;
    this->userSet = true;
}

std::string User::getUsername() const
{
    return this->username;
}

void User::setPass(bool val)
{
    this->passGiven = val;
}

bool User::isPassSet() const
{
    return this->passGiven;
}

bool User::isNickSet() const
{
    return this->nickSet;
}

bool User::isUserSet() const
{
    return this->userSet;
}

void User::setAuthenticated(bool val)
{
    this->authenticated = val;
}

bool User::isAuthenticated() const
{
    return this->authenticated;
}

void User::joinChannel(Channel *channel)
{
    if (channel)
        channel->addUser(this);
}

void User::setCurrentChannel(Channel *channel)
{
    this->currentChannel = channel;
}

Channel* User::getCurrentChannel() const
{
    return this->currentChannel;
}