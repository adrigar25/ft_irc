/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdList.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:09:56 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:16:19 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_HPP
#define CMD_LIST_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdList : public ICommand {
	public:
		CmdList() {}
		virtual ~CmdList() {}
		virtual void execute(RequestContext &ctx);
};

#endif
