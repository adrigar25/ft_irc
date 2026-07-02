/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:48 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:49 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PASS_HPP
#define CMD_PASS_HPP

#include "commands/ICommand_bonus.hpp"
#include "../RequestContext_bonus.hpp"
#include <string>

class CmdPass : public ICommand {
public:
	CmdPass() {}
	virtual ~CmdPass() {}
	virtual void execute(RequestContext &ctx);
};

#endif
