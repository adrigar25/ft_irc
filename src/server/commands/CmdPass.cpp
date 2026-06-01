#include "commands/CmdPass.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <string>
#include <cstring>

#include "line_utils.h"

static bool applyPassword(RequestContext &ctx, const std::string &pass)
{
    Server* srv = ctx.services.getServer();
    if (srv && srv->validatePassword(pass)) {
        ctx.sender->setPass(true);
        ctx.services.sendToUser(ctx.sender, std::string("Password accepted"));
        return true;
    }
    ctx.services.sendToUser(ctx.sender, std::string("Invalid password"));
    return false;
}

void CmdPass::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string params = trimParam(ctx.rawLine);

    if (params.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("461 PASS :Not enough parameters"));
        return;
    }
    applyPassword(ctx, params);
}
