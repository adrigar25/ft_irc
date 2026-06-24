/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:06:09 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:06:11 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_MODE_HPP
#define CMD_MODE_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdMode : public ICommand {
public:
	CmdMode() {}
	virtual ~CmdMode() {}
	virtual void execute(RequestContext &ctx);
};

#endif
