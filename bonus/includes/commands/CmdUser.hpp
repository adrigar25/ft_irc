/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:21 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:22 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_USER_HPP
#define CMD_USER_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUser : public ICommand {
public:
	CmdUser() {}
	virtual ~CmdUser() {}
	virtual void execute(RequestContext &ctx);
};

#endif
