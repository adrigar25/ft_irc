/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:44 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:20:45 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdQuit.hpp"
#include "RequestContext.hpp"
#include "replies/Replies.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include "LineUtils.hpp"
#include <string>

void CmdQuit::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;

	User *user = ctx.sender;
	std::string quitMsg = "Client Quit";

	if (!ctx.rawLine.empty() && ctx.rawLine[0] == ':')
		quitMsg = trim(ctx.rawLine.substr(1), " \r");

	std::string prefix = ctx.services.getUserPrefix(user);
	std::string msg = RPL_QUIT(prefix, quitMsg);

	std::map<std::string, Channel*> userChannels = user->getChannels();

	for (std::map<std::string, Channel*>::const_iterator it = userChannels.begin();
		 it != userChannels.end(); ++it)
	{
		ctx.services.sendToChannel(it->second, msg, user);
	}

	ctx.services.channels().removeUserFromAllChannels(user);
	ctx.services.users().remove(user->getSocket());
}

