/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNames.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:06:06 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:06:07 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_NAMES_HPP
#define CMD_NAMES_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdNames : public ICommand {
public:
	CmdNames() {}
	virtual ~CmdNames() {}
	virtual void execute(RequestContext &ctx);
};

#endif
