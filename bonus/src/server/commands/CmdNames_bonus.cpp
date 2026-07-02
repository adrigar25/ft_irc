/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:52 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:34:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdNames_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "Server_bonus.hpp"
#include "Channel_bonus.hpp"
#include <sstream>
#include <vector>

/**
 * @brief Executes the NAMES command.
 * - Handles the NAMES command for listing users in channels.
 * - If no channels are specified, lists users in all channels.
 * - If channels are specified, lists users in those channels.
 * @param ctx The request context.
 */
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
