/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdCap.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:26 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:24:31 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdCap_bonus.hpp"
#include "RequestContext_bonus.hpp"
#include "Services_bonus.hpp"
#include "User_bonus.hpp"
#include <string>
#include <sstream>

#include "LineUtils_bonus.hpp"

/**
 * @brief Executes the CAP command
 *  - Handles the CAP command for capability negotiation.
 *  - Supports the following subcommands:
 *    - LS: Lists the server's capabilities.
 *    - REQ: Requests specific capabilities.
 *    - END: Ends the capability negotiation.
 * @param ctx The request context
 */
void CmdCap::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	std::string params = trim(ctx.rawLine, " \r");
	if (params.empty()) return;

	std::istringstream iss(params);
	std::string sub;
	iss >> sub;
	if (sub == "LS") {
		ctx.services.sendToUser(ctx.sender, std::string("CAP * LS : multi-prefix"));
		return;
	}
	if (sub == "REQ") {
		std::string rest;
		std::getline(iss, rest);
		if (!rest.empty() && rest[0] == ' ')
			rest.erase(0,1);
		if (rest.empty())
			return;
		ctx.services.sendToUser(ctx.sender, std::string("CAP * ACK ") + rest);
		return;
	}
	if (sub == "END") {
		return;
	}
}
