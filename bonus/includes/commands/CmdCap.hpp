/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdCap.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:09:02 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:35 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_CAP_HPP
#define CMD_CAP_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdCap : public ICommand {
	public:
		CmdCap() {}
		virtual ~CmdCap() {}
		virtual void execute(RequestContext &ctx);
};

#endif
