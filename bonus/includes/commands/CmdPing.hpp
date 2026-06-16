/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:24 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:57 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PING_HPP
#define CMD_PING_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPing : public ICommand {
	public:
		CmdPing() {}
		virtual ~CmdPing() {}
		virtual void execute(RequestContext &ctx);
};

#endif
