/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ChannelManager.cpp								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:12:59 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 01:56:21 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ChannelManager.hpp"
#include "IrcConnection.hpp"
#include <algorithm>

ChannelManager::ChannelManager(IRCConnection *irc) : irc(irc) {}

ChannelManager::~ChannelManager() {}

void ChannelManager::joinChannel(const std::string &channel)
{
	if (this->isInChannel(channel))
		return;
	if (this->irc)
	{
		this->irc->sendRaw("JOIN " + channel);
		this->joined.push_back(channel);
	}
}

void ChannelManager::partChannel(const std::string &channel)
{
	if (!this->isInChannel(channel))
		return;
	if (this->irc)
		this->irc->sendRaw("PART " + channel + " :Leaving");
	deleteChannel(channel);
}

bool ChannelManager::checkUserOP(const std::string &channel, const std::string &user)
{
	if (!this->isInChannel(channel))
		return false;
	if (this->irc)
	{
		std::string m = "NAMES " + channel;
		this->irc->sendRaw(m);
		std::string response = this->irc->recvLine();

		if(response.find("@" + user) != std::string::npos || response.find("@+" + user) != std::string::npos)
			return true;
	}
	return false;
}

bool ChannelManager::isOnlyUserInChannel(const std::string &channel)
{
	if (!this->isInChannel(channel))
		return false;
	if (this->irc)
	{
		std::string m = "NAMES " + channel;
		this->irc->sendRaw(m);
		std::string response = this->irc->recvLine();

		size_t pos = response.find(" :");
		if (pos != std::string::npos)
		{
			std::string users = response.substr(pos + 2);
			size_t count = std::count(users.begin(), users.end(), ' ') + 1;
			return count == 1;
		}
	}
	return false;
}

bool ChannelManager::isInChannel(const std::string &channel) const
{
	return std::find(this->joined.begin(), this->joined.end(), channel) != this->joined.end();
}

void ChannelManager::deleteChannel(const std::string &channel)
{
	this->joined.erase(std::remove(this->joined.begin(), this->joined.end(), channel), this->joined.end());
}
