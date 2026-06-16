/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:02 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:20 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <string>
#include <map>
#include "Channel.hpp"
#include "User.hpp"

class ChannelManager {
	public:
		ChannelManager();
		~ChannelManager();

		Channel* getChannel(const std::string &name) const;
		void createChannel(const std::string &name, User* creator);
		void deleteChannel(const std::string &name);
		void removeUserFromChannel(const std::string &channelName, User* user);
		void removeUserFromAllChannels(User* user);
		std::map<std::string, Channel*>& getAll();
		const std::map<std::string, Channel*>& getAll() const;
		void clear();

	private:
		std::map<std::string, Channel*> channels;
};

#endif
