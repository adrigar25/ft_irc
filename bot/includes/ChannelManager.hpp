/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:16:02 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/02 23:16:03 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_MANAGER_HPP
#define CHANNEL_MANAGER_HPP

#include <string>
#include <vector>

class IRCConnection;

class ChannelManager {
public:
	ChannelManager(IRCConnection *conn);
	~ChannelManager();

	void joinChannel(const std::string &channel);
	void partChannel(const std::string &channel);
	bool isInChannel(const std::string &channel) const;
	void sendToChannel(const std::string &channel, const std::string &message);

private:
	IRCConnection *conn;
	std::vector<std::string> joined;
};

#endif
