#include "commands/CmdCap.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>
#include <sstream>

#include "line_utils.h"

void CmdCap::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string params = trimParam(ctx.rawLine);
    if (params.empty()) return;

    std::istringstream iss(params);
    std::string sub;
    iss >> sub;
    if (sub == "LS") {
        ctx.services.sendToUser(ctx.sender, std::string("CAP * LS :"));
        return;
    }
    if (sub == "LIST") {
        ctx.services.sendToUser(ctx.sender, std::string("CAP * LIST :"));
        return;
    }
    if (sub == "REQ") {
        std::string rest;
        std::getline(iss, rest);
        if (!rest.empty() && rest[0] == ' ') rest.erase(0,1);
        if (rest.empty()) return;
        ctx.services.sendToUser(ctx.sender, std::string("CAP * ACK ") + rest);
        return;
    }
    if (sub == "END") {
        return;
    }
}
