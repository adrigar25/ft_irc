/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNames.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:04 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:12 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_NAMES_HPP
#define CMD_NAMES_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdNames : public ICommand {
	public:
		CmdNames() {}
		virtual ~CmdNames() {}
		virtual void execute(RequestContext &ctx);
};

#endif
