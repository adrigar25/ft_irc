/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMsg.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:28 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/19 11:06:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_MSG_HPP
#define CMD_MSG_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdMsg : public ICommand
{
	private:
		bool _isNotice;
	public:
		CmdMsg(bool isNotice);
		void execute(RequestContext &ctx);
};

#endif
