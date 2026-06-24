/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ChannelAccessors.cpp							   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:37:19 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/12 18:16:09 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <sstream>

void Channel::setIsInviteOnly(bool val)
{
	this->isInviteOnly = val;
}

void Channel::setUserLimit(int limit)
{
	this->userLimit = limit;
}

void Channel::setTopic(const std::string &t)
{
	this->topic = t;
}

void Channel::setKey(const std::string &k)
{
	this->key = k;
}

void Channel::setKeyRequired(bool val)
{
	this->keyRequired = val;
}

void Channel::setIsSecret(bool val)
{
	this->isSecret = val;
}

void Channel::setTopicProtected(bool val)
{
	this->topicProtected = val;
}
void Channel::setModerated(bool val)
{
	this->isModerated = val;
}

/**
 * @brief Devuelve el número de usuarios en el canal.
 */
int Channel::getUserCount() const
{
	return this->userCount;
}


/**
 * @brief Devuelve el límite de usuarios del canal, o -1 si no hay límite.
 */
int Channel::getUserLimit() const
{
	return this->userLimit;
}


/**
 * @brief Indica si el canal es privado.
 */
bool Channel::getIsInviteOnly() const
{
	return this->isInviteOnly;
}


/**
 * @brief Devuelve el nombre del canal.
 */
std::string Channel::getName() const
{
	return this->name;
}


/**
 * @brief Devuelve el mapa de usuarios del canal.
 */
std::map<int, User*>& Channel::getUsers()
{
	return this->users;
}


/**
 * @brief Devuelve la clave del canal.
 */
std::string Channel::getKey() const
{
	return this->key;
}

/**
 * @brief Devuelve si el canal requiere clave.
 */
bool Channel::getKeyRequired() const
{
	return this->keyRequired;
}

bool Channel::getIsSecret() const
{
	return this->isSecret;
}

bool Channel::getTopicProtected() const
{
	return this->topicProtected;
}

std::string Channel::getTopic() const
{
	return this->topic;
}

std::map<int, User*>& Channel::getOperators()
{
	return this->operators;
}

std::map<int, User*>& Channel::getVoiceUsers()
{
	return this->voiceUsers;
}

std::map<int, User*>& Channel::getInvitedUsers()
{
	return this->invitedUsers;
}

std::map<int, User*>& Channel::getBannedUsers()
{
	return this->bannedUsers;
}

bool Channel::getIsModerated() const
{
	return this->isModerated;
}

void Channel::getChannelModes(std::string &outModes, std::vector<std::string> &outParams) const
{
	if (this->getIsInviteOnly())
		outModes += 'i';
	if (this->getTopicProtected())
		outModes += 't';
	if (this->getIsModerated())
		outModes += 'm';
	if (this->getKeyRequired())
	{
		outModes += 'k';
		outParams.push_back(this->getKey());
	}
	if (this->getUserLimit() != -1)
	{
		std::stringstream ss;
		ss << this->getUserLimit();
		outModes += 'l';
		outParams.push_back(ss.str());
	}
}
