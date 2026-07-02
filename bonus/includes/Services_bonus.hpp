/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:19 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 19:04:16 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <string>
#include "managers/UserManager_bonus.hpp"
#include "managers/ChannelManager_bonus.hpp"
#include "RequestContext_bonus.hpp"

class Server;
class User;

class Services {
	private:
		Server* server;
		UserManager userManager;
		ChannelManager channelManager;
	public:
		Services(Server* srv);
		~Services();
		Server* getServer() const;
		UserManager& users();
		ChannelManager& channels() ;
		const UserManager& Users();
		const ChannelManager& channels() const;
		std::string getServerName() const;
		void sendToUser(User* user, const std::string &message);
		void sendToChannel(Channel* channel, const std::string &message, const User* exclude = NULL);
		void sendResponse(RequestContext &ctx, const std::string &reply);
		void sendNamesList(RequestContext &ctx, const User *target, Channel *channel);
		std::string getUserPrefix(const User* user) const;
		std::string getServerPrefix() const;

};

#endif
