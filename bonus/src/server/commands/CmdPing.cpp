/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:20:34 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:20:36 by adriescr         ###   ########.fr       */
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
