/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUnknown.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:25 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:26 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_UNKNOWN_HPP
#define CMD_UNKNOWN_HPP

#include "commands/ICommand_bonus.hpp"
#include "../RequestContext_bonus.hpp"
#include <string>

class CmdUnknown : public ICommand {
public:
	CmdUnknown() {}
	virtual ~CmdUnknown() {}
	virtual void execute(RequestContext &ctx);
};

#endif
