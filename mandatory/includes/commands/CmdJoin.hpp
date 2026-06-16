/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:07:01 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:07:02 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_JOIN_HPP
#define CMD_JOIN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdJoin : public ICommand {
public:
	CmdJoin() {}
	virtual ~CmdJoin() {}
	virtual void execute(RequestContext &ctx);
};

#endif
