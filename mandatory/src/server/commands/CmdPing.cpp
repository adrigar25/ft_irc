/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:34 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:02:35 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPing.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>

#include "LineUtils.hpp"
#include "replies/Replies.hpp"

void CmdPing::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string token = trim(ctx.rawLine, " \r");
	if (token.empty()) return;
	ctx.services.sendToUser(ctx.sender, RPL_PONG(token));
}
