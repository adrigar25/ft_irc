/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:12:59 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:13:24 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"
#include "IrcConnection.hpp"
#include <algorithm>

ChannelManager::ChannelManager(IRCConnection *conn): conn(conn) {}

ChannelManager::~ChannelManager() {}

void ChannelManager::joinChannel(const std::string &channel)
{
	if (this->isInChannel(channel))
		return;
	if (this->conn)
		this->conn->sendRaw("JOIN " + channel);
	this->joined.push_back(channel);
}

void ChannelManager::partChannel(const std::string &channel)
{
	if (!this->isInChannel(channel))
		return;
	if (this->conn)
		this->conn->sendRaw("PART " + channel + " :Leaving");
	this->joined.erase(std::remove(this->joined.begin(), this->joined.end(), channel), this->joined.end());
}

bool ChannelManager::isInChannel(const std::string &channel) const
{
	return (std::find(this->joined.begin(), this->joined.end(), channel) != this->joined.end());
}

void ChannelManager::sendToChannel(const std::string &channel, const std::string &message)
{
	if (this->conn)
		this->conn->sendRaw("PRIVMSG " + channel + " :" + message);
}
