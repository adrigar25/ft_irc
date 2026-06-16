/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:08 by agarcia          ###   ########.fr       */
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
