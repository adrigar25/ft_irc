/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:19 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:04:20 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <string>
#include "managers/UserManager.hpp"
#include "managers/ChannelManager.hpp"
#include "RequestContext.hpp"

class Server;
class User;

class Services {
public:
	Services(Server* srv);
	~Services();

	Server* getServer() const;

private:
	Server* server;
	UserManager userManager;
	ChannelManager channelManager;
public:
	UserManager& users();
	ChannelManager& channels() ;
	const UserManager& users() const;
	const ChannelManager& channels() const;
	std::string getServerName() const;
	void sendToUser(User* user, const std::string &message);
	void sendToChannel(Channel* channel, const std::string &message, User* exclude = NULL);
	void sendResponse(RequestContext &ctx, const std::string &reply);
	void sendNamesList(RequestContext &ctx, User *target, Channel *channel);
	std::string getUserPrefix(User* user) const;
	std::string getServerPrefix() const;

};

#endif
