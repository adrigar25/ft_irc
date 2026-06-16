/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUnknown.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:40 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:40 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_UNKNOWN_HPP
#define CMD_UNKNOWN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUnknown : public ICommand {
	public:
		CmdUnknown() {}
		virtual ~CmdUnknown() {}
		virtual void execute(RequestContext &ctx);
};

#endif
