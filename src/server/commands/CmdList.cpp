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
    std::string response = "Channels:\n";
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel *channel = it->second;
        if(channel->getIsInviteOnly() || channel->getIsSecret())
            continue;
        std::ostringstream oss;
        oss << it->second->getUserCount();
        response += it->first + " (" + oss.str() + " users)\n";
    }
    ctx.services.sendToUser(ctx.sender, response);
}

void CmdList::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    sendChannelsList(ctx);
}
