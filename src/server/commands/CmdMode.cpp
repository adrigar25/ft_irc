/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:52:57 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/09 17:57:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdMode.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "LineUtils.hpp"

#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>

static bool isNumber(const std::string &s)
{
    for (size_t i = 0; i < s.size(); ++i)
        if (!std::isdigit(s[i]))
            return false;
    return !s.empty();
}

static void sendMode(RequestContext &ctx, Channel *ch, const std::string &channelName,
                     char sign, char mode, const std::string &param)
{
    std::string msg = ":"
        + ctx.sender->getNickname()
        + " MODE "
        + channelName
        + " ";

    msg += sign;
    msg += mode;

    if (!param.empty())
        msg += " " + param;

    msg += "\r\n";

    ctx.services.sendToChannel(ch, msg);
}

/* ===================== HANDLERS ===================== */

static void modeI(Channel *ch, bool add)
{
    ch->setIsInviteOnly(add);
}

static void modeT(Channel *ch, bool add)
{
    ch->setTopicProtected(add);
}

static void modeM(Channel *ch, bool add)
{
    ch->setModerated(add);
}

static void modeK(Channel *ch, bool add, const std::string &key)
{
    if (add)
    {
        ch->setKey(key);
        ch->setKeyRequired(true);
    }
    else
    {
        ch->setKey("");
        ch->setKeyRequired(false);
    }
}

static void modeL(Channel *ch, bool add, int limit)
{
    if (add)
        ch->setUserLimit(limit);
    else
        ch->setUserLimit(-1);
}

static void modeO(Channel *ch, bool add, User *u)
{
    if (add)
        ch->changeRole(u, "operator");
    else
        ch->removeRole(u, "operator");
}

static void modeV(Channel *ch, bool add, User *u)
{
    if (add)
        ch->changeRole(u, "voice");
    else
        ch->removeRole(u, "voice");
}

static void modeB(Channel *ch, bool add, User *u)
{
    if (add)
        ch->banUser(u);
    else
        ch->unbanUser(u);
}

/* ===================== EXEC ===================== */

void CmdMode::execute(RequestContext &ctx)
{
    if (!ctx.sender || ctx.rawLine.empty())
        return;

    std::vector<std::string> t = split(ctx.rawLine, ' ');

    if (t.size() < 2)
    {
        ctx.services.sendResponse(ctx, "461", "Not enough parameters");
        return;
    }

    std::string channelName = t[0];
    std::string modes = t[1];

    Channel *ch = ctx.services.channels().getChannel(channelName);
    if (!ch)
    {
        ctx.services.sendResponse(ctx, "403", channelName + " :No such channel");
        return;
    }

    if (!ch->isUserOperator(ctx.sender))
    {
        ctx.services.sendResponse(ctx, "482", channelName + " :You're not channel operator");
        return;
    }

    std::vector<std::string> params;
    for (size_t i = 2; i < t.size(); ++i)
        params.push_back(t[i]);

    size_t p = 0;
    bool add = true;

    for (size_t i = 0; i < modes.size(); ++i)
    {
        char m = modes[i];

        if (m == '+') { add = true; continue; }
        if (m == '-') { add = false; continue; }

        std::string param;

        switch (m)
        {
            case 'i':
                modeI(ch, add);
                sendMode(ctx, ch, channelName, add ? '+' : '-', 'i', "");
                break;

            case 't':
                modeT(ch, add);
                sendMode(ctx, ch, channelName, add ? '+' : '-', 't', "");
                break;

            case 'm':
                modeM(ch, add);
                sendMode(ctx, ch, channelName, add ? '+' : '-', 'm', "");
                break;

            case 'k':
                if (add)
                {
                    if (p >= params.size())
                    {
                        ctx.services.sendResponse(ctx, "461", "MODE :Not enough parameters");
                        return;
                    }
                    param = params[p++];
                }
                modeK(ch, add, param);
                sendMode(ctx, ch, channelName, add ? '+' : '-', 'k', param);
                break;

            case 'l':
                if (add)
                {
                    if (p >= params.size())
                    {
                        ctx.services.sendResponse(ctx, "461", "MODE :Not enough parameters");
                        return;
                    }
                    if (!isNumber(params[p]))
                    {
                        ctx.services.sendResponse(ctx, "696", channelName + " :Invalid limit");
                        return;
                    }
                    int limit = std::atoi(params[p++].c_str());
                    modeL(ch, add, limit);
                    sendMode(ctx, ch, channelName, '+', 'l', params[p - 1]);
                }
                else
                {
                    modeL(ch, add, -1);
                    sendMode(ctx, ch, channelName, '-', 'l', "");
                }
                break;

            case 'o':
            case 'v':
            case 'b':
            {
                if (p >= params.size())
                {
                    ctx.services.sendResponse(ctx, "461", "MODE :Not enough parameters");
                    return;
                }

                User *u = ctx.services.users().findByNick(params[p++]);
                if (!u)
                {
                    ctx.services.sendResponse(ctx, "401", params[p - 1] + " :No such user");
                    continue;
                }

                if (m == 'o') modeO(ch, add, u);
                if (m == 'v') modeV(ch, add, u);
                if (m == 'b') modeB(ch, add, u);

                std::string nick = u->getNickname();
                sendMode(ctx, ch, channelName, add ? '+' : '-', m, nick);
                break;
            }

            default:
                ctx.services.sendResponse(ctx, "472", std::string(1, m) + " :Unknown mode");
                break;
        }
    }
}
