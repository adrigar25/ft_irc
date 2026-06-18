/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:11 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/18 01:38:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdQuit.hpp"
#include "RequestContext.hpp"
#include "replies/Replies.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include "LineUtils.hpp"
#include <string>
#include <unistd.h>

void CmdQuit::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;

	User *user = ctx.sender;
	int fd = user->getSocket();
	std::string quitMsg = "Client Quit";

	if (!ctx.rawLine.empty() && ctx.rawLine[0] == ':')
		quitMsg = trim(ctx.rawLine.substr(1), " \r");

	std::string prefix = ctx.services.getUserPrefix(user);

	for (std::map<std::string, Channel *>::const_iterator it = user->getChannels().begin(); it != user->getChannels().end(); ++it)
		ctx.services.sendToChannel(it->second, RPL_QUIT(prefix, quitMsg), user);

	ctx.services.channels().removeUserFromAllChannels(user);
	ctx.services.users().remove(fd);
	close(fd);
}

