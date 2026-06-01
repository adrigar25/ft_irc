#include "commands/CmdMode.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"

#include <string>

static void handleMode(RequestContext &ctx)
{
    ctx.services.sendToUser(ctx.sender, std::string("MODE command not implemented yet"));
}

void CmdMode::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    handleMode(ctx);
}
