/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:55 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:56 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PART_HPP
#define CMD_PART_HPP

#include "commands/ICommand_bonus.hpp"
#include "../RequestContext_bonus.hpp"
#include <string>

class CmdPart : public ICommand {
public:
	CmdPart() {}
	virtual ~CmdPart() {}
	virtual void execute(RequestContext &ctx);
};

#endif
