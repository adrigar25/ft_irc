#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <iostream>

/**
 * @brief Añade un usuario al canal, validando restricciones (invite-only, límite, baneos).
 *
 * Lanza excepciones específicas si el usuario no puede unirse.
 * @param user Puntero al `User` a añadir.
 */
void Channel::addUser(User *user)
{
    int fd = user->getSocket();
    if (this->isInviteOnly && this->invitedUsers.find(fd) == this->invitedUsers.end())
        throw IrcException(IRC_ERR_CHANNEL_INVITE_ONLY, "Channel is invite-only, user must be invited to join");
    if (this->users.find(fd) != this->users.end())
        throw IrcException(IRC_ERR_USER_ALREADY_EXISTS, "User already exists in channel");
    if (this->userCount >= this->userLimit && this->userLimit != -1)
        throw IrcException(IRC_ERR_CHANNEL_FULL, "Channel is full");
    if (this->bannedUsers.find(fd) != this->bannedUsers.end())
        throw IrcException(IRC_ERR_USER_BANNED, "User is banned from this channel");
    this->users.insert(std::make_pair(fd, user));
    this->userCount++;
    std::cout << "User " << user->getNickname() << " added to channel " << this->name << std::endl;
}


/**
 * @brief Elimina un usuario del canal y actualiza el contador.
 * @param user Puntero al `User` a eliminar.
 */
void Channel::deleteUser(User *user)
{
    int fd = user->getSocket();
    if (this->users.find(fd) == this->users.end())
        throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
    this->users.erase(fd);
    this->userCount--;
    std::cout << "User " << user->getNickname() << " removed from channel " << this->name << std::endl;
}


/**
 * @brief Añade `user` al mapa `userMap` si no existe.
 */
void Channel::addUserToMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) != userMap.end())
        return;
    userMap.insert(std::make_pair(fd, user));
}

/**
 * @brief Elimina `user` del mapa `userMap` si existe.
 */
void Channel::deleteUserFromMap(std::map<int, User*> &userMap, User *user)
{
    int fd = user->getSocket();
    if(userMap.find(fd) == userMap.end())
        return;
    userMap.erase(fd);
}

/**
 * @brief Marca a `user` como invitado en el canal.
 */
void Channel::inviteUser(User *user)
{
    addUserToMap(this->invitedUsers, user);
}

void Channel::banUser(User *user)
{
    addUserToMap(this->bannedUsers, user);
}


void Channel::unbanUser(User *user)
{
    deleteUserFromMap(this->bannedUsers, user);
}

