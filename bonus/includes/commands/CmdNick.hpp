/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:09 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:09 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_NICK_HPP
#define CMD_NICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdNick : public ICommand {
	public:
		CmdNick() {}
		virtual ~CmdNick() {}
		virtual void execute(RequestContext &ctx);
};

#endif
