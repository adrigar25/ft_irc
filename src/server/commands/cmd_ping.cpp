#include "commands/CmdPing.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>

static std::string trimParam(const std::string &s)
{
    std::string r = s;
    if (!r.empty() && r[0] == ' ') r.erase(0,1);
    if (!r.empty() && r[r.size() - 1] == '\r') r.erase(r.size() - 1, 1);
    return r;
}

/**
 * @brief Responde a un PING con el PONG correspondiente (eco de parámetros).
 *
 * El cliente puede enviar `PING <token>` o `PING :<token>`. Este handler
 * devuelve `PONG <token>` exactamente con los mismos parámetros (salvo
 * recortes de CR/LF y espacios iniciales).
 */
void CmdPing::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string token = trimParam(ctx.rawLine);
    if (token.empty())
        return;
    ctx.services.sendToUser(ctx.sender, std::string("PONG ") + token);
}
