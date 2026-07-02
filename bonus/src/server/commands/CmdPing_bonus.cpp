/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:02:34 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:35:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPing_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "User_bonus.hpp"
#include <string>

#include "LineUtils_bonus.hpp"
#include "replies/Replies_bonus.hpp"

/**
 * @brief Executes the PING command.
 * - Handles the PING command for responding to server pings.
 * - Sends a PONG response back to the user with the provided token.
 * @param ctx The request context.
 */
void CmdPing::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string token = trim(ctx.rawLine, " \r");
	if (token.empty()) return;
	ctx.services.sendToUser(ctx.sender, RPL_PONG(token));
}
