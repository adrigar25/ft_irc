/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:43 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PING_HPP
#define CMD_PING_HPP

#include "commands/ICommand_bonus.hpp"
#include "../RequestContext_bonus.hpp"
#include <string>

class CmdPing : public ICommand {
public:
	CmdPing() {}
	virtual ~CmdPing() {}
	virtual void execute(RequestContext &ctx);
};

#endif
