#include "commands/CmdNick.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include "managers/UserManager.hpp"
#include <string>
#include <cstring>


/**
 * @brief Extrae y limpia el nick de la línea cruda.
 * @param raw Línea de parámetros.
 * @return nick limpio.
 */
static std::string extractNick(const std::string &raw)
{
    std::string nick = raw;
    if (!nick.empty() && nick[0] == ' ') nick.erase(0, 1);
    if (!nick.empty() && nick[nick.size() - 1] == '\r') nick.erase(nick.size() - 1, 1);
    return nick;
}

/**
 * @brief Comprueba disponibilidad del nick y responde en caso de conflicto.
 * @return true si el nick está disponible para este usuario.
 */
static bool nickAvailable(RequestContext &ctx, const std::string &nick)
{
    User* existing = ctx.services.users().findByNick(nick);
    if (existing && existing != ctx.sender) {
        ctx.services.sendToUser(ctx.sender, std::string("433 NICK :Nickname is already in use"));
        return false;
    }
    return true;
}

/**
 * @brief Aplica el nick al usuario y confirma al cliente.
 */
static void applyNick(RequestContext &ctx, const std::string &nick)
{
    ctx.sender->setNickname(nick);
    ctx.services.sendToUser(ctx.sender, std::string("Nickname set"));
}

void CmdNick::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::string nick = extractNick(ctx.rawLine);

    if (nick.empty()) {
        ctx.services.sendToUser(ctx.sender, std::string("431 NICK :No nickname given"));
        return;
    }

    if (!nickAvailable(ctx, nick)) return;
    applyNick(ctx, nick);
}
