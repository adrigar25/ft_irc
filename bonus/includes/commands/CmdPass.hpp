/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:21 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:00 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PASS_HPP
#define CMD_PASS_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPass : public ICommand {
	public:
		CmdPass() {}
		virtual ~CmdPass() {}
		virtual void execute(RequestContext &ctx);
};

#endif
