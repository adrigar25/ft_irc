/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:32 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:50 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_QUIT_HPP
#define CMD_QUIT_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdQuit : public ICommand {
	public:
		CmdQuit() {}
		virtual ~CmdQuit() {}
		virtual void execute(RequestContext &ctx);
};

#endif
