#include "commands/CmdKick.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"

#include <string>
#include <unistd.h>
#include <cstring>

/**
 * @brief Parsea parámetros de KICK en canal, usuario y razón.
 */
static void parseKickParams(const std::string &raw, std::string &outChannel, std::string &outTargetNick, std::string &outReason)
{
    outChannel.clear();
    outTargetNick.clear();
    outReason.clear();
    size_t sp1 = raw.find(' ');
    if (sp1 == std::string::npos) {
        outChannel = raw;
        return;
    }
    outChannel = raw.substr(0, sp1);
    size_t sp2 = raw.find(' ', sp1 + 1);
    if (sp2 == std::string::npos) {
        outTargetNick = raw.substr(sp1 + 1);
        return;
    }
    outTargetNick = raw.substr(sp1 + 1, sp2 - sp1 - 1);
    outReason = raw.substr(sp2 + 1);
    if (!outReason.empty() && outReason[0] == ' ') outReason.erase(0,1);
    if (!outReason.empty() && outReason[0] == ':') outReason.erase(0,1);
    if (!outReason.empty() && outReason[outReason.size() - 1] == '\r') outReason.erase(outReason.size() - 1, 1);
}

/**
 * @brief Valida condiciones para poder kickear. Envía errores al cliente si fallan.
 */
static bool validateKick(RequestContext &ctx, const std::string &channelName, const std::string &targetNick, Channel *&outChannel, User *&outTarget)
{
    outChannel = ctx.services.channels().getChannel(channelName);
    if (!outChannel) {
        ctx.services.sendToUser(ctx.sender, std::string("403 ") + channelName + " :No such channel");
        return false;
    }

    outTarget = ctx.services.users().findByNick(targetNick);
    if (!outTarget) {
        ctx.services.sendToUser(ctx.sender, std::string("401 ") + targetNick + " :No such nick/channel");
        return false;
    }

    if (!outChannel->isUserOperator(ctx.sender)) {
        ctx.services.sendToUser(ctx.sender, std::string("482 ") + channelName + " :You're not channel operator");
        return false;
    }

    if (!outChannel->hasUser(outTarget)) {
        ctx.services.sendToUser(ctx.sender, std::string("441 ") + outTarget->getNickname() + " " + channelName + " :They aren't on that channel");
        return false;
    }

    return true;
}

/**
 * @brief Ejecuta el KICK: notifica canal y remueve al usuario.
 */
static void doKick(RequestContext &ctx, Channel *channel, User *target, const std::string &reason)
{
    // Build KICK line with prefix
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) std::strcpy(hostname, "localhost");
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string out = std::string(":") + ctx.sender->getNickname() + "!" + uname + "@" + hostname + " KICK " + channel->getName() + " " + target->getNickname();
    if (!reason.empty()) out += std::string(" :") + reason;
    out += "\r\n";

    // broadcast to channel (include all)
    ctx.services.sendToChannel(channel, out, NULL);

    // remove user from channel
    target->leaveChannel(channel);
}

void CmdKick::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    Channel* channel = NULL;
    User* targetUser = NULL;
    std::string channelName;
    std::string targetNick;
    std::string reason;
    parseKickParams(ctx.rawLine, channelName, targetNick, reason);

    if (channelName.empty() || targetNick.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 KICK :Not enough parameters"));
        return;
    }

    if (!validateKick(ctx, channelName, targetNick, channel, targetUser)) return;

    doKick(ctx, channel, targetUser, reason);
}
