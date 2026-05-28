#include "commands/CmdList.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <sstream>


/**
 * @brief Construye y envía al usuario la lista de canales disponibles.
 * @param ctx Contexto de la petición.
 */
static void sendChannelsList(RequestContext &ctx)
{
    const std::map<std::string, Channel*>& channels = ctx.services.channels().getAll();
    std::string response = "Channels:\n";
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        std::ostringstream oss;
        oss << it->second->getUserCount();
        response += it->first + " (" + oss.str() + " users)\n";
    }
    ctx.services.sendToUser(ctx.sender, response);
}

void CmdList::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    sendChannelsList(ctx);
}
