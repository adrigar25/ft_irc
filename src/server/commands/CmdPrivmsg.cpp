/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/09 18:18:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdPrivmsg.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "LineUtils.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

static bool parsePrivmsgParams(const std::string &params, std::vector<std::string> &outTargets, std::string &outMsg)
{
    std::vector<std::string> parts = split(params, ' ');
    if (parts.size() < 2) return false;

    outTargets = split(parts[0], ',');
    
    if(params.find(" :") != std::string::npos) {
        outMsg = params.substr(params.find(" :") + 2);
    } else {
        outMsg = parts[1];
    }
    
    return true;
}

static void dispatchPrivmsg(RequestContext &ctx, const std::string &target, const std::string &msg)
{

    if(target.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("411 PRIVMSG :No recipient given"));
        return;
    }

    std::string out = ctx.services.getUserPrefix(ctx.sender) + RPL_PRIVMSG(target, msg);

    if(target[0] == '#') {
        Channel *channel = ctx.services.channels().getChannel(target);
        if (!channel) {
            ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
            return;
        }
        if(!channel->hasUser(ctx.sender)) {
            ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
            return;
        }
        if(channel->getIsModerated() && !channel->isUserVoice(ctx.sender) && !channel->isUserOperator(ctx.sender)) {
            ctx.services.sendResponse(ctx, ERR_CANNOTSENDTOCHAN(ctx.sender->getNickname(), target));
            return;
        }
        ctx.services.sendToChannel(channel, out, ctx.sender);
    } else {
        User *dest = ctx.services.users().findByNick(target);
        if (!dest) {
            ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), target));
            return;
        }
        ctx.services.sendToUser(dest, out);
    }
}

void CmdPrivmsg::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    const std::string &params = ctx.rawLine;
    std::vector<std::string> targets;
    std::string msg;
    
    if(parts.empty()) {
        ctx.services.sendResponse(ctx, ERR_NORECIPIENT(ctx.sender->getNickname(), "PRIVMSG"));
        return;
    }else if(parts.size() == 1 || (parts.size() > 1 && parts[1][0] != ':')) {
        ctx.services.sendResponse(ctx, ERR_NOTEXTTOSEND(ctx.sender->getNickname(), "PRIVMSG"));
        return;
    }
    for (size_t i = 0; i < targets.size(); ++i) {
        const std::string &target = targets[i];
        if(target.empty()) {
            ctx.services.sendToUser(ctx.sender, std::string("411 PRIVMSG :No recipient given"));
            return;
        }
        if(msg.empty()) {
            ctx.services.sendToUser(ctx.sender, std::string("412 PRIVMSG :No text to send"));
            return;
        }
        std::string out = buildPrivmsgOut(ctx, target, msg);
        dispatchPrivmsg(ctx, target, out);
    }
}
