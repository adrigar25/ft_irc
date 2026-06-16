/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:09:41 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:27 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_JOIN_HPP
#define CMD_JOIN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdJoin : public ICommand {
	public:
		CmdJoin() {}
		virtual ~CmdJoin() {}
		virtual void execute(RequestContext &ctx);
};

#endif
