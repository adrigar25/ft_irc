#include "commands/CmdQuit.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>

void CmdQuit::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    User *user = ctx.sender;
    ctx.services.sendToUser(user, std::string("Goodbye!"));

    ctx.services.channels().removeUserFromAllChannels(user);
    ctx.services.users().remove(user->getSocket());

}
