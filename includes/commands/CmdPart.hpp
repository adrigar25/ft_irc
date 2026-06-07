/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:35:25 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:35:27 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PART_HPP
#define CMD_PART_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPart : public ICommand {
public:
    CmdPart() {}
    virtual ~CmdPart() {}
    virtual void execute(RequestContext &ctx);
};

#endif
