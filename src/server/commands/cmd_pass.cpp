#include "commands/CmdPass.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Server.hpp"
#include <string>
#include <cstring>

/**
 * @brief Limpia espacios iniciales y CR final de un parámetro.
 */
static std::string trimParam(const std::string &s)
{
    std::string r = s;
    if (!r.empty() && r[0] == ' ') r.erase(0,1);
    if (!r.empty() && r[r.size() - 1] == '\r') r.erase(r.size() - 1, 1);
    return r;
}

/**
 * @brief Valida la contraseña contra el servidor y aplica el estado en el usuario.
 * @return true si la contraseña fue aceptada.
 */
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
