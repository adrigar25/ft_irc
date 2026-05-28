/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: GitHub Copilot                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by copilot           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PRIVMSG_HPP
#define CMD_PRIVMSG_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPrivmsg : public ICommand {
public:
    CmdPrivmsg() {}
    virtual ~CmdPrivmsg() {}
    virtual void execute(RequestContext &ctx);
};

#endif
