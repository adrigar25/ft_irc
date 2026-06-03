#include "commands/CmdInvite.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <string>
#include <cstring>

static void parseInviteParams(const std::string &params, std::string &outTargetNick, std::string &outChannelName)
{
    size_t sp = params.find(' ');
    outTargetNick = (sp == std::string::npos) ? params : params.substr(0, sp);
    outChannelName = (sp == std::string::npos) ? std::string() : params.substr(sp + 1);
    if (!outChannelName.empty() && outChannelName[0] == ' ') outChannelName.erase(0,1);
    if (!outChannelName.empty() && outChannelName[outChannelName.size() - 1] == '\r') outChannelName.erase(outChannelName.size() - 1, 1);
}

static bool canInvite(RequestContext &ctx, User* targetUser, Channel* channel)
{
    if (!targetUser || !channel) {
        ctx.services.sendToUser(ctx.sender, std::string("Invalid user or channel"));
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

    User* targetUser = ctx.services.users().findByNick(targetNick);
    Channel* channel = ctx.services.channels().getChannel(channelName);

    if (!canInvite(ctx, targetUser, channel)) return;
    channel->inviteUser(targetUser);

    ctx.services.sendToUser(targetUser, ":" + ctx.sender->getNickname() + " INVITE " + targetUser->getNickname() + " :" + channel->getName());
    ctx.services.sendToUser(ctx.sender, ":server 341 " + ctx.sender->getNickname() + " " + targetUser->getNickname() + " :" + channel->getName());
}
