/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdList.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:45 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:37:45 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdList.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <sstream>

static void sendChannelsList(RequestContext &ctx)
{
    const std::map<std::string, Channel*>& channels = ctx.services.channels().getAll();
    std::string serverName = ctx.services.getServerName();
    ctx.services.sendToUser(ctx.sender, ":" + serverName + " 321 " + ctx.sender->getNickname() + " Channel :Users Name");
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel *channel = it->second;
        if(channel->getIsInviteOnly() || channel->getIsSecret())
            continue;
        std::ostringstream oss;
        oss << it->second->getUserCount();
        std::string topic = channel->getTopic().empty() ? "-" : channel->getTopic();
        ctx.services.sendToUser(ctx.sender, ":" + serverName + " 322 " + ctx.sender->getNickname() + " " + it->first + " " + oss.str() + " :" + topic);
    }
    ctx.services.sendToUser(ctx.sender, ":" + serverName + " 323 " + ctx.sender->getNickname() + " :End of /LIST");
}

void CmdList::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    sendChannelsList(ctx);
}
