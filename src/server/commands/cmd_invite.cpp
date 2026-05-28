#include "commands/CmdInvite.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <string>
#include <cstring>

/**
 * @brief Parsea los parámetros del comando INVITE en nick y canal.
 * @param params La línea cruda de parámetros.
 * @param outTargetNick Resultado: nick del usuario a invitar.
 * @param outChannelName Resultado: nombre del canal.
 */
static void parseInviteParams(const std::string &params, std::string &outTargetNick, std::string &outChannelName)
{
    size_t sp = params.find(' ');
    outTargetNick = (sp == std::string::npos) ? params : params.substr(0, sp);
    outChannelName = (sp == std::string::npos) ? std::string() : params.substr(sp + 1);
    if (!outChannelName.empty() && outChannelName[0] == ' ') outChannelName.erase(0,1);
    if (!outChannelName.empty() && outChannelName[outChannelName.size() - 1] == '\r') outChannelName.erase(outChannelName.size() - 1, 1);
}

/**
 * @brief Valida las condiciones necesarias para invitar.
 * @return true si se puede proceder con la invitación, false y responde al usuario en caso contrario.
 */
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

/**
 * @brief Realiza la invitación una vez validados los parámetros.
 */
static void doInvite(RequestContext &ctx, User* targetUser, Channel* channel)
{
    channel->inviteUser(targetUser);
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
    doInvite(ctx, targetUser, channel);
}
