/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:35:39 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:35:41 by agarcia          ###   ########.fr       */
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
