/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:59 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:37:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPing.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>

#include "LineUtils.hpp"

void CmdPing::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string token = trimParam(ctx.rawLine);
    if (token.empty()) return;
    ctx.services.sendToUser(ctx.sender, std::string("PONG ") + token);
}
