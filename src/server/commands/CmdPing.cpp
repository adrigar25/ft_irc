#include "commands/CmdPing.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>

#include "line_utils.h"

void CmdPing::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string token = trimParam(ctx.rawLine);
    if (token.empty()) return;
    ctx.services.sendToUser(ctx.sender, std::string("PONG ") + token);
}
