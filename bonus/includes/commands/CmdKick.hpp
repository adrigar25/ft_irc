/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:09:51 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:22 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_KICK_HPP
#define CMD_KICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdKick : public ICommand {
	public:
		CmdKick() {}
		virtual ~CmdKick() {}
		virtual void execute(RequestContext &ctx);
};

#endif
