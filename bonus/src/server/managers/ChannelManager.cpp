/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:49 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/18 02:03:43 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "managers/ChannelManager.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Exceptions.hpp"
#include <iostream>

ChannelManager::ChannelManager() : channels()
{
}
ChannelManager::~ChannelManager() {}

void ChannelManager::createChannel(const std::string &name, User *creator)
{
	if (channels.find(name) != channels.end())
		throw IrcException(IRC_ERR_CHANNEL_ALREADY_EXISTS, std::string("Channel already exists: ") + name);

	if (name.empty() || name[0] != '#' || name.find(' ') != std::string::npos)
		throw IrcException(IRC_ERR_BAD_CHANNEL_NAME, std::string("Invalid channel name: ") + name);
	Channel *ch = new Channel(name, creator);
	channels.insert(std::make_pair(name, ch));
	if (creator)
	{
		try
		{
			creator->joinChannel(ch, "");
			ch->changeRole(creator, "operator");
		}
		catch (const std::exception &e)
		{
			channels.erase(name);
			delete ch;
			throw;
		}
	}
}

void ChannelManager::deleteChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = channels.find(name);
	if (it == channels.end())
		return;
	Channel *ch = it->second;
	channels.erase(it);
	delete ch;
}

void ChannelManager::removeUserFromChannel(const std::string &channelName, User *user)
{
	Channel *ch = getChannel(channelName);
	if (!ch)
		return;
	user->leaveChannel(ch);
	if (ch->isEmpty())
		deleteChannel(channelName);
}

void ChannelManager::removeUserFromAllChannels(User *user)
{
	std::map<std::string, Channel *> userChannels = user->getChannels();
	for (std::map<std::string, Channel *>::iterator it = userChannels.begin(); it != userChannels.end(); ++it)
		removeUserFromChannel(it->first, user);
}

Channel *ChannelManager::getChannel(const std::string &name) const
{
	std::map<std::string, Channel *>::const_iterator it = channels.find(name);
	if (it != channels.end())
		return it->second;
	return NULL;
}

std::map<std::string, Channel *> &ChannelManager::getAll() { return channels; }

void ChannelManager::clear()
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		delete it->second;
	channels.clear();
}

const std::map<std::string, Channel *> &ChannelManager::getAll() const { return channels; }

