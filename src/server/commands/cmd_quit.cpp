#include "commands/CmdQuit.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>


void CmdQuit::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    User *user = ctx.sender;
    // send goodbye while the user object and its socket are still valid
    ctx.services.sendToUser(user, std::string("Goodbye!"));

    // then remove the user from all channels and from the user manager
    ctx.services.channels().removeUserFromAllChannels(user);
    ctx.services.users().remove(user->getSocket());

}
