/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:28 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:54 by adriescr         ###   ########.fr       */
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
