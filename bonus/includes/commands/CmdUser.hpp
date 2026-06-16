/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:45 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:37 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_USER_HPP
#define CMD_USER_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUser : public ICommand {
	public:
		CmdUser() {}
		virtual ~CmdUser() {}
		virtual void execute(RequestContext &ctx);
};

#endif
