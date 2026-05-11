#include "Channel.hpp"
#include "User.hpp"
#include <map>
#include <iostream>

Channel::Channel(std::string name, User *creator): name(name), userCount(0), isPrivate(false), userLimit(-1)
{
    std::cout << "Channel " << this->name << " created by " << creator->getNickname() << std::endl;
    this->addUser(creator);
    this->addUserToMap(this->operators, creator);
}

Channel::~Channel()
{
    std::cout << "Channel " << this->name << " deleted" << std::endl;
};

void Channel::addUser(User *user)
{
    int fd = user->getSocket();
    if (this->isPrivate && this->invitedUsers.find(fd) == this->invitedUsers.end())
        throw privateChannelException();
    if (this->users.find(fd) != this->users.end())
        throw userAlreadyExistsException();
    if (this->userCount >= this->userLimit && this->userLimit != -1)
        throw channelFullException();
    this->users.insert(std::make_pair(fd, user));
    this->userCount++;
    user->setCurrentChannel(this);
    std::cout << "User " << user->getNickname() << " added to channel " << this->name << std::endl;
}

void Channel::removeUser(User *user)
{
    int fd = user->getSocket();
    if (this->users.find(fd) == this->users.end())
        throw userNotFoundException();
    this->users.erase(fd);
    this->userCount--;
    user->setCurrentChannel(NULL);
    std::cout << "User " << user->getNickname() << " removed from channel " << this->name << std::endl;
}

void Channel::addUserToMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) != userMap.end())
        return; // User already has this role, do nothing
    userMap.insert(std::make_pair(fd, user));
}

void Channel::removeUserFromMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) == userMap.end())
        return; // User doesn't have this role, do nothing
    userMap.erase(fd);
}

void Channel::changeRole(User *user, std::string role)
{
    int fd = user->getSocket();
    if(this->users.find(fd) == this->users.end())
        throw userNotFoundException();
    std::map<std::string, std::map<int, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    roles.insert(std::make_pair(std::string("excepted"), &this->exceptedUsers));

    if(roles.find(role) == roles.end())
        throw roleNotFoundException();
    roles[role]->insert(std::make_pair(fd, user));
}

int Channel::getUserCount() const
{
    return this->userCount;
}

int Channel::getUserLimit() const
{
    return this->userLimit;
}

bool Channel::getIsPrivate() const
{
    return this->isPrivate;
}

std::string Channel::getName() const
{
    return this->name;
}

bool Channel::hasUser(User *user) const
{
    int fd = user->getSocket();
    return this->users.find(fd) != this->users.end();
}

bool Channel::isUserOperator(User *user) const
{
    int fd = user->getSocket();
    return this->operators.find(fd) != this->operators.end();
}

bool Channel::isUserVoice(User *user) const
{
    int fd = user->getSocket();
    return this->voiceUsers.find(fd) != this->voiceUsers.end();
}

bool Channel::isUserInvited(User *user) const
{
    int fd = user->getSocket();
    return this->invitedUsers.find(fd) != this->invitedUsers.end();
}

bool Channel::isUserBanned(User *user) const
{
    int fd = user->getSocket();
    return this->bannedUsers.find(fd) != this->bannedUsers.end();
}

bool Channel::isUserExcepted(User *user) const
{
    int fd = user->getSocket();
    return this->exceptedUsers.find(fd) != this->exceptedUsers.end();
}

bool Channel::canUserJoin(User *user) const
{
    if (this->getUserLimit() != -1 && this->getUserCount() >= this->getUserLimit())
        return false;
    if(this->getIsPrivate() && !this->isUserInvited(user))
        return false;
    if(this->isUserBanned(user))
        return false;
    return true;
}

const std::map<int, User*>& Channel::getUsers() const
{
    return this->users;
}