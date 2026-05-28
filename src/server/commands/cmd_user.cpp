#include "commands/CmdUser.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstring>

/**
 * @brief Parsea la línea de USER en nombre y realname.
 */
static void parseUserLine(const std::string &raw, std::string &outUsername, std::string &outReal)
{
    std::istringstream iss(raw);
    std::string username, mode, unused, real;
    iss >> username >> mode >> unused;
    std::getline(iss, real);
    if (!real.empty() && real[0] == ' ') real.erase(0,1);
    if (!real.empty() && real[0] == ':') real.erase(0,1);
    outUsername = username;
    outReal = real;
}

/**
 * @brief Aplica username al usuario y envía el welcome si corresponde.
 */
static void setUserAndMaybeWelcome(RequestContext &ctx, const std::string &username, const std::string &real)
{
    ctx.sender->setUsername(username);
    ctx.services.sendToUser(ctx.sender, std::string("Username set"));

    if (ctx.sender->isPassSet() && ctx.sender->isNickSet() && ctx.sender->isUserSet()) {
        ctx.sender->setAuthenticated(true);
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) != 0)
            std::strcpy(hostname, "localhost");
        std::string welcome = std::string(":") + hostname + " 001 " + ctx.sender->getNickname() + " :Welcome to " + hostname;
        ctx.services.sendToUser(ctx.sender, welcome);
    }
}

void CmdUser::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    if (ctx.rawLine.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 USER :Not enough parameters"));
        return;
    }
    std::string username, real;
    parseUserLine(ctx.rawLine, username, real);
    setUserAndMaybeWelcome(ctx, username, real);
}
