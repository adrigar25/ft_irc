#include "commands/CmdPart.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <vector>
#include <string>
#include <sstream>
/**
 * @brief Parsea la lista de canales separados por ',' y valida nombres.
 * @param channelsPart Parte de la línea que contiene los nombres de canales.
 * @param out Vector donde se ponen los nombres válidos.
 * @param ctx Contexto para enviar errores al usuario si hay nombres inválidos.
 */
static void parseChannelList(const std::string &channelsPart, std::vector<std::string> &out, RequestContext &ctx)
{
    std::istringstream ciss(channelsPart);
    std::string value;
    while (std::getline(ciss, value, ','))
    {
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!value.empty() && (value[0] == '#' || value[0] == '&' || value[0] == '+' || value[0] == '!'))
            out.push_back(value);
        else if (!value.empty())
            ctx.services.sendToUser(ctx.sender, std::string("Invalid channel name: ") + value);
    }
}

/**
 * @brief Hace que el usuario salga de un canal si existe.
 */
static void partFromChannel(RequestContext &ctx, const std::string &channelName)
{
    Channel *channel = ctx.services.channels().getChannel(channelName);
    if (channel) ctx.sender->leaveChannel(channel);
}

/**
 * @brief Separa la porción de canales y el mensaje opcional de la línea RAW.
 * @param raw Línea de parámetros brutos.
 * @param outChannels Resultado: substring con la lista de canales.
 * @param outMsg Resultado: substring con el resto (mensaje), vacío si no existe.
 */
static void splitChannelsAndMsg(const std::string &raw, std::string &outChannels, std::string &outMsg)
{
    outChannels = raw;
    outMsg.clear();
    size_t sp = raw.find(' ');
    if (sp != std::string::npos) {
        outChannels = raw.substr(0, sp);
        outMsg = raw.substr(sp + 1);
    }
}

void CmdPart::execute(RequestContext &ctx)
{
    if (!ctx.sender) return;
    std::vector<std::string> channelNames;

    std::string channelsPart;
    std::string msgPart;
    splitChannelsAndMsg(ctx.rawLine, channelsPart, msgPart);

    parseChannelList(channelsPart, channelNames, ctx);

    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        partFromChannel(ctx, *it);
    }
}
