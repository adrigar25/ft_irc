/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNameList.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:40 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 12:23:58 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "LineUtils.hpp"
#include <iostream>
#include <cerrno>
#include <sys/socket.h>
#include <vector>

void Server::sendNamesList(User *target, Channel *channel)
{
    std::string serverName = this->getHostname();
    const std::map<int, User*>& usersMap = channel->getUsers();
    std::string namesList;
    for (std::map<int, User*>::const_iterator uit = usersMap.begin(); uit != usersMap.end(); ++uit) {
        if (!namesList.empty()) 
            namesList += " ";
        if (channel->isUserOperator(uit->second))
            namesList += "@";
        if(channel->isUserVoice(uit->second))
            namesList += "+";
        namesList += uit->second->getNickname();
    }
    std::string namesReply = std::string(":") + serverName + " 353 " + target->getNickname() + " = " + channel->getName() + " :" + namesList + "\r\n";
    sendToUser(target, namesReply);

    std::string endNames = std::string(":") + serverName + " 366 " + target->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
    sendToUser(target, endNames);
}