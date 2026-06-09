/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:21:37 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdInvite.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "LineUtils.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <string>
#include <cstring>

static void parseInviteParams(const std::string &params, std::string &outTargetNick, std::string &outChannelName)
{
    std::vector<std::string> parts = split(trim(params, " \r"), ' ');
    if (parts.size() >= 2) {
        outTargetNick = parts[0];
        outChannelName = parts[1];
    }
}

static bool canInvite(RequestContext &ctx, User* targetUser, Channel* channel)
{
    if (!targetUser) {
        ctx.services.sendToUser(ctx.sender, std::string("No such user"));
        return false;
    }
    if (!channel) {
        ctx.services.sendToUser(ctx.sender, std::string("No such channel"));
        return false;
    }
    if (!channel->isUserOperator(ctx.sender)) {
        ctx.services.sendToUser(ctx.sender, std::string("You must be an operator to invite users"));
        return false;
    }
    if (channel->hasUser(targetUser)) {
        ctx.services.sendToUser(ctx.sender, std::string("User is already in the channel"));
        return false;
    }
    return true;
}

void CmdInvite::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string targetNick;
    std::string channelName;
    parseInviteParams(ctx.rawLine, targetNick, channelName);

    if (targetNick.empty() || channelName.empty()) 
    {
        ctx.services.sendToUser(ctx.sender, std::string("461 INVITE :Not enough parameters"));
        return;
    }

    User* targetUser = ctx.services.users().findByNick(targetNick);
    Channel* channel = ctx.services.channels().getChannel(channelName);

    if (!canInvite(ctx, targetUser, channel)) return;
    channel->inviteUser(targetUser);

    ctx.services.sendToUser(targetUser, ":" + ctx.sender->getNickname() + " INVITE " + targetUser->getNickname() + " :" + channel->getName());
    ctx.services.sendToUser(ctx.sender, ":server 341 " + ctx.sender->getNickname() + " " + targetUser->getNickname() + " :" + channel->getName());
}
