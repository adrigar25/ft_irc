/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:05 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 14:40:55 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdQuit.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>

void CmdQuit::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    User *user = ctx.sender;
    ctx.services.sendToUser(user, std::string("Goodbye!"));

    ctx.services.channels().removeUserFromAllChannels(user);
    ctx.services.users().remove(user->getSocket());

}
