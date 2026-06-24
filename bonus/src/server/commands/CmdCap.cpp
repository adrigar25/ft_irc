/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdCap.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:26 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:03:27 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdCap.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>
#include <sstream>

#include "LineUtils.hpp"

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
