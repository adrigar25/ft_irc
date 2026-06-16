#include "commands/CmdKick.hpp"
#include "RequestContext.hpp"
#include "LineUtils.hpp"
#include "Services.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include "replies/Replies.hpp"

static bool validateKick(
    RequestContext &ctx,
    const std::string &channelName,
    const std::string &targetNick,
    Channel *&outChannel,
    User *&outTarget)
{
    outChannel = ctx.services.channels().getChannel(channelName);
    if (!outChannel)
    {
        ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
        return false;
    }

    outTarget = ctx.services.users().findByNick(targetNick);
    if (!outTarget)
    {
        ctx.services.sendResponse(ctx, ERR_NOSUCHNICK(ctx.sender->getNickname(), targetNick));
        return false;
    }

    if (!outChannel->isUserOperator(ctx.sender))
    {
        ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
        return false;
    }

    if (!outChannel->hasUser(outTarget))
    {
        ctx.services.sendResponse(ctx, ERR_USERNOTINCHANNEL(ctx.sender->getNickname(), targetNick, channelName));
        return false;
    }

    return true;
}

static void doKick(RequestContext &ctx, Channel *channel, User *target, const std::string &reason)
{
    std::string serverName = ctx.services.getServerName();
    std::string uname = ctx.sender->getUsername();
    if (uname.empty())
        uname = "~";

    std::string msg = RPL_KICK(ctx.services.getUserPrefix(ctx.sender), channel->getName(), target->getNickname(), reason.empty() ? "No reason" : reason);
    ctx.services.sendToChannel(channel, msg, NULL);
    target->leaveChannel(channel);
}

static void handleKick1N(RequestContext &ctx, const std::string &channelName, const std::vector<std::string> &users, const std::string &reason)
{
    for(size_t i = 0; i < users.size(); ++i)
    {
        std::string nick = trim(users[i], " \r");

        if (nick.empty())
            continue;

        Channel *channel = NULL;
        User *target = NULL;

        if (!validateKick(ctx, channelName, nick, channel, target))
            continue;

        doKick(ctx, channel, target, reason);
    }
}

static void handleKickN1(RequestContext &ctx, const std::vector<std::string> &channels, const std::string &nick, const std::string &reason)
{
    for(size_t i = 0; i < channels.size(); ++i)
    {
        std::string chName = trim(channels[i], " \r");

        if (chName.empty())
            continue;

        Channel *channel = NULL;
        User *target = NULL;

        if (!validateKick(ctx, chName, nick, channel, target))
            continue;

        doKick(ctx, channel, target, reason);
    }
}

void CmdKick::execute(RequestContext &ctx)
{
    if (!ctx.sender)
        return;

    std::vector<std::string> parts = split(trim(ctx.rawLine, " \r"), ' ');
    if (parts.size() < 2)
    {
        ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));
        return;
    }

    std::vector<std::string> channels = split(trim(parts[0], " "), ',');
    std::vector<std::string> users = split(trim(parts[1], " "), ',');

    std::string reason = "";
    size_t pos = ctx.rawLine.find(" :");
    if (pos != std::string::npos)
        reason = ctx.rawLine.substr(pos + 2);

    if (channels.empty() || users.empty())
    {
        ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));
        return;
    }

    if(channels.size() == 1)
        handleKick1N(ctx, channels[0], users, reason);
    else if(users.size() == 1)
        handleKickN1(ctx, channels, users[0], reason);
    else
        ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "KICK"));

}
