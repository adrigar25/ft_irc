/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:35:04 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 16:33:33 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef CMD_KICK_HPP
#define CMD_KICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdKick : public ICommand {
public:
    CmdKick() {}
    virtual ~CmdKick() {}
    virtual void execute(RequestContext &ctx);
};

#endif
