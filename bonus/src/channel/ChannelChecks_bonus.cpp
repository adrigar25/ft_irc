/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelChecks.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:43 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:40:24 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel_bonus.hpp"
#include "User_bonus.hpp"
#include "Exceptions_bonus.hpp"
#include "replies/Replies_bonus.hpp"
#include <map>
#include <fnmatch.h>

/**
 * @brief Builds a user mask for pattern matching.
 * @param user The user for whom to build the mask.
 * @return The constructed user mask.
 */
static std::string buildUserMask(const User *user)
{
	return user->getNickname() + "!" + user->getUsername() + "@" + user->getHost();
}

/**
 * @brief Comprueba si `user` pertenece al canal.
 */
bool Channel::hasUser(const User *user) const
{
	int fd = user->getSocket();
	return this->users.find(fd) != this->users.end();
}


/**
 * @brief Comprueba si `user` es operador del canal.
 */
bool Channel::isUserOperator(const User *user) const
{
	int fd = user->getSocket();
	return this->operators.find(fd) != this->operators.end();
}


/**
 * @brief Comprueba si `user` tiene voz en el canal.
 */
bool Channel::isUserVoice(const User *user) const
{
	int fd = user->getSocket();
	return this->voiceUsers.find(fd) != this->voiceUsers.end();
}


/**
 * @brief Comprueba si `user` fue invitado al canal.
 */
bool Channel::isUserInvited(const User *user) const
{
	int fd = user->getSocket();
	return this->invitedUsers.find(fd) != this->invitedUsers.end();
}


/**
 * @brief Comprueba si `user` está baneado del canal.
 */
bool Channel::isUserBanned(const User *user) const
{
	std::string mask = buildUserMask(user);
	for (std::vector<std::string>::const_iterator it = this->bannedMasks.begin(); it != this->bannedMasks.end(); ++it)
	{
		if (fnmatch(it->c_str(), mask.c_str(), 0) == 0)
				return true;
	}
	return false;
}

/**
 * @brief Comprueba si el canal no tiene usuarios.
 */
bool Channel::isEmpty() const
{
	return this->userCount == 0;
}


/**
 * @brief Comprueba si `user` puede unirse al canal según límites, privacy y baneos.
 */
void Channel::canUserJoin(const User *user, const std::string &userKey) const
{
	if (this->getUserLimit() > 0 && this->getUserCount() >= this->getUserLimit())
		throw IrcException(IRC_ERR_CHANNEL_FULL, "Channel is full");
	if(this->getIsInviteOnly() && !this->isUserInvited(user))
		throw IrcException(IRC_ERR_ISINVITEONLYCHAN, "Channel is invite only");
	if(this->isUserBanned(user))
		throw IrcException(IRC_ERR_BANNEDFROMCHAN, "You are banned from this channel");
	if(this->getKeyRequired() && this->getKey() != userKey)
		throw IrcException(IRC_ERR_INCORRECT_CHANNEL_KEY, "Incorrect channel key");
}
