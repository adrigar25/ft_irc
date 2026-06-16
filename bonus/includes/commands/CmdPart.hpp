/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:14 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:05 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PART_HPP
#define CMD_PART_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPart : public ICommand {
	public:
		CmdPart() {}
		virtual ~CmdPart() {}
		virtual void execute(RequestContext &ctx);
};

#endif
