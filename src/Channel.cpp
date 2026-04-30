#include "Channel.hpp"
#include "User.hpp"
#include <map>
#include <iostream>

Channel::Channel(std::string name, User *creator): name(name), userCount(0),isPrivate(false), userLimit(-1)
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
    if(this->isPrivate && this->invitedUsers.find(user->getNickname()) == this->invitedUsers.end())
        throw privateChannelException();
    if(this->users.find(user->getNickname()) != this->users.end())
        throw userAlreadyExistsException();
    if(this->userCount >= this->userLimit && this->userLimit != -1)
        throw channelFullException();
    this->users.insert(std::make_pair(user->getNickname(), user));
    this->userCount++;
    std::cout << "User " << user->getNickname() << " added to channel " << this->name << std::endl;
}

void Channel::removeUser(User *user)
{
    if(this->users.find(user->getNickname()) == this->users.end())
        throw userNotFoundException();
    this->users.erase(user->getNickname());
    this->userCount--;
    std::cout << "User " << user->getNickname() << " removed from channel " << this->name << std::endl;
}

void Channel::addUserToMap(std::map<std::string, User*> &userMap, User *user)
{
    if(userMap.find(user->getNickname()) != userMap.end())
        return; // User already has this role, do nothing
    userMap.insert(std::make_pair(user->getNickname(), user));
}

void Channel::removeUserFromMap(std::map<std::string, User*> &userMap, User *user)
{
    if(userMap.find(user->getNickname()) == userMap.end())
        return; // User doesn't have this role, do nothing
    userMap.erase(user->getNickname());
}

void Channel::changeRole(User *user, std::string role)
{
    if(this->users.find(user->getNickname()) == this->users.end())
        throw userNotFoundException();
    std::map<std::string, std::map<std::string, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    roles.insert(std::make_pair(std::string("excepted"), &this->exceptedUsers));

    if(roles.find(role) == roles.end())
        throw roleNotFoundException();
    roles[role]->insert(std::make_pair(user->getNickname(), user));
}

