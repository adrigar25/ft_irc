/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:21:15 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:21:15 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandDispatcher.hpp"
#include "commands/ICommand.hpp"
#include "RequestContext.hpp"
#include <iostream>

CommandDispatcher::CommandDispatcher()
{
}

CommandDispatcher::~CommandDispatcher()
{
	for (std::map<std::string, ICommand*>::iterator it = handlers.begin(); it != handlers.end(); ++it) {
		delete it->second;
	}
	handlers.clear();
}

void CommandDispatcher::registerHandler(const std::string& cmd, ICommand* handler)
{
	handlers[cmd] = handler;
}

bool CommandDispatcher::hasHandler(const std::string& cmd) const
{
	return handlers.find(cmd) != handlers.end();
}

void CommandDispatcher::dispatch(const std::string& cmd, RequestContext &ctx) const
{
	std::map<std::string, ICommand*>::const_iterator it = handlers.find(cmd);
	if (it == handlers.end())
		return;
	ICommand* h = it->second;
	if (!h) return;
	h->execute(ctx);
}
