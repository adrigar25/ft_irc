/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:35:30 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:35:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PASS_HPP
#define CMD_PASS_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPass : public ICommand {
public:
    CmdPass() {}
    virtual ~CmdPass() {}
    virtual void execute(RequestContext &ctx);
};

#endif
