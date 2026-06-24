/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:52 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:02:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdNames.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>

void CmdNames::execute(RequestContext &ctx)
{
	if (ctx.rawLine.empty())
		return;
	std::istringstream iss(ctx.rawLine);
	std::vector<std::string> channelNames;
	for(std::string word; iss >> word; ) {
		if (word[0] == '#')
			channelNames.push_back(word);
	}
	if(channelNames.empty())
		return;
	
	for(size_t i = 0; i < channelNames.size(); ++i) {
		std::string &name = channelNames[i];
		if (name.empty() || name[0] != '#')
			continue;
		Channel *channel = ctx.services.channels().getChannel(name);
		if (!channel)
			continue;
		ctx.services.sendNamesList(ctx, ctx.sender, channel);
	}
}
