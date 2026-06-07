/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Services.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:56 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:36:58 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <string>
#include "managers/UserManager.hpp"
#include "managers/ChannelManager.hpp"

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
    UserManager& users() { return userManager; }
    ChannelManager& channels() { return channelManager; }
    const UserManager& users() const { return userManager; }
    const ChannelManager& channels() const { return channelManager; }
    std::string getServerName() const;
    void sendToUser(User* user, const std::string &message);
    void sendToChannel(Channel* channel, const std::string &message, User* exclude = NULL);

};

#endif
