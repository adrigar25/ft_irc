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

static std::string buildPrivmsgOut(RequestContext &ctx, const std::string &target, const std::string &msg)
{
    std::string serverName = ctx.services.getServerName();
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string out = std::string(":") + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " PRIVMSG " + target + " :" + msg + "\r\n";
    return out;
}

static void dispatchPrivmsg(RequestContext &ctx, const std::string &target, const std::string &out)
{
    if(target.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("411 PRIVMSG :No recipient given"));
        return;
    }

    if(target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!') {
        Channel *channel = ctx.services.channels().getChannel(target);
        if (!channel) {
            ctx.services.sendToUser(ctx.sender, std::string("403 ") + target + " :No such channel");
            return;
        }
        if(!channel->hasUser(ctx.sender)) {
            ctx.services.sendToUser(ctx.sender, std::string("404 ") + target + " :Cannot send to channel");
            return;
        }
        if(channel->getIsModerated() && !channel->isUserVoice(ctx.sender) && !channel->isUserOperator(ctx.sender)) {
            ctx.services.sendToUser(ctx.sender, std::string("404 ") + target + " :Cannot send to channel (moderated)");
            return;
        }
        ctx.services.sendToChannel(channel, out, ctx.sender);
    } else {
        User *dest = ctx.services.users().findByNick(target);
        if (!dest) {
            ctx.services.sendToUser(ctx.sender, std::string("401 ") + target + " :No such nick/channel");
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
    if (!parsePrivmsgParams(params, targets, msg)) {
        ctx.services.sendToUser(ctx.sender, std::string("461 PRIVMSG :Not enough parameters"));
        return;
    }
    for (size_t i = 0; i < targets.size(); ++i) {
        const std::string &target = targets[i];
        if(target.empty() || msg.empty()) {
            ctx.services.sendToUser(ctx.sender, std::string("461 PRIVMSG :Not enough parameters"));
            return;
        }
        std::string out = buildPrivmsgOut(ctx, target, msg);
        dispatchPrivmsg(ctx, target, out);
    }
}
