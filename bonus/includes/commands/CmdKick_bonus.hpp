/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:06:53 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:06:56 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_KICK_HPP
#define CMD_KICK_HPP

#include "commands/ICommand_bonus.hpp"
#include "../RequestContext_bonus.hpp"
#include <string>

class CmdKick : public ICommand {
public:
	CmdKick() {}
	virtual ~CmdKick() {}
	virtual void execute(RequestContext &ctx);
};

#endif
