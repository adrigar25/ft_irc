/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:17 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:38:18 by agarcia          ###   ########.fr       */
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
