/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdList.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:02 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:26:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdList_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "Server_bonus.hpp"
#include <sstream>
#include "replies/Replies_bonus.hpp"

/**
 * @brief Sends a list of all channels to the user
 * @param ctx The request context
 */
static void sendChannelsList(RequestContext &ctx)
{
	const std::map<std::string, Channel*>& channels = ctx.services.channels().getAll();
	std::string serverName = ctx.services.getServerName();
	ctx.services.sendResponse(ctx, RPL_LISTSTART(ctx.sender->getNickname()));
	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		const Channel *channel = it->second;
		if(channel->getIsInviteOnly() || channel->getIsSecret())
			continue;
		std::ostringstream oss;
		oss << it->second->getUserCount();
		std::string topic = channel->getTopic().empty() ? "-" : channel->getTopic();
		ctx.services.sendResponse(ctx, RPL_LIST(ctx.sender->getNickname(), it->first, oss.str(), topic));
	}
	ctx.services.sendResponse(ctx, RPL_ENDOFLIST(ctx.sender->getNickname()));
}

/**
 * @brief Executes the LIST command
 * 	- Handles the LIST command for listing channels.
 * 	- Sends a list of all channels to the user, excluding invite-only and secret channels.
 * 	- Sends appropriate responses for the start and end of the list.
 * @param ctx The request context
 */
void CmdList::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	sendChannelsList(ctx);
}
