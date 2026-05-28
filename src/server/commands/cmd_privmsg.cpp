#include "commands/CmdPrivmsg.hpp"

#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

/**
 * @brief Parsea parámetros de PRIVMSG en target y mensaje.
 * @return true si el parse fue correcto, false y responde con error si no lo fue.
 */
static bool parsePrivmsgParams(const std::string &params, std::string &outTarget, std::string &outMsg)
{
    size_t sp = params.find(' ');
    outTarget = (sp == std::string::npos) ? params : params.substr(0, sp);

    if (sp != std::string::npos) {
        outMsg = params.substr(sp + 1);
        if (!outMsg.empty() && outMsg[0] == ' ') outMsg.erase(0,1);
        if (!outMsg.empty() && outMsg[0] == ':') outMsg.erase(0,1);
        if (!outMsg.empty() && outMsg[outMsg.size() - 1] == '\r') outMsg.erase(outMsg.size() - 1, 1);
        return true;
    }
    return false;
}

/**
 * @brief Construye la línea completa de PRIVMSG con prefijo de usuario.
 */
static std::string buildPrivmsgOut(RequestContext &ctx, const std::string &target, const std::string &msg)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
        std::strcpy(hostname, "localhost");
    std::string uname = ctx.sender->getUsername();
    if (uname.empty()) uname = "~";
    std::string out = std::string(":") + ctx.sender->getNickname() + "!" + uname + "@" + hostname + " PRIVMSG " + target + " :" + msg + "\r\n";
    return out;
}

/**
 * @brief Envía el PRIVMSG al destino (canal o usuario), respondiendo con errores si procede.
 */
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
    std::string target;
    std::string msg;
    if (!parsePrivmsgParams(params, target, msg)) {
        ctx.services.sendToUser(ctx.sender, std::string("461 PRIVMSG :Not enough parameters"));
        return;
    }
    if(target.empty() || msg.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 PRIVMSG :Not enough parameters"));
        return;
    }

    std::string out = buildPrivmsgOut(ctx, target, msg);
    dispatchPrivmsg(ctx, target, out);
}
