/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdCap.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:07:10 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:07:13 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_CAP_HPP
#define CMD_CAP_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdCap : public ICommand {
public:
	CmdCap() {}
	virtual ~CmdCap() {}
	virtual void execute(RequestContext &ctx);
};

#endif
