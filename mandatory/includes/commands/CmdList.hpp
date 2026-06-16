/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdList.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:35:07 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:35:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_HPP
#define CMD_LIST_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdList : public ICommand {
public:
    CmdList() {}
    virtual ~CmdList() {}
    virtual void execute(RequestContext &ctx);
};

#endif
