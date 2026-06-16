/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvite.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:09:11 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:31 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_INVITE_HPP
#define CMD_INVITE_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdInvite : public ICommand {
	public:
		CmdInvite() {}
		virtual ~CmdInvite() {}
		virtual void execute(RequestContext &ctx);
};

#endif
