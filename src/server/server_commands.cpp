#include "Server.hpp"
#include "User.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <utility>
#include "CommandDispatcher.hpp"
#include "commands/CmdPrivmsg.hpp"
#include "commands/CmdPass.hpp"
#include "commands/CmdNick.hpp"
#include "commands/CmdUser.hpp"
#include "commands/CmdJoin.hpp"
#include "commands/CmdPart.hpp"
#include "commands/CmdPing.hpp"
#include "commands/CmdQuit.hpp"
#include "commands/CmdList.hpp"
#include "commands/CmdInvite.hpp"
#include "commands/CmdKick.hpp"
#include "commands/CmdMode.hpp"
#include "RequestContext.hpp"

// File-scoped dispatcher getter: registers handlers on first call.
static CommandDispatcher& getDispatcher()
{
    static CommandDispatcher dispatcher;
    static bool inited = false;
    if (!inited) {
        dispatcher.registerHandler("PASS", new CmdPass());
        dispatcher.registerHandler("NICK", new CmdNick());
        dispatcher.registerHandler("USER", new CmdUser());
        dispatcher.registerHandler("JOIN", new CmdJoin());
        dispatcher.registerHandler("PART", new CmdPart());
        dispatcher.registerHandler("PRIVMSG", new CmdPrivmsg());
        dispatcher.registerHandler("QUIT", new CmdQuit());
        dispatcher.registerHandler("PING", new CmdPing());
        dispatcher.registerHandler("LIST", new CmdList());
        dispatcher.registerHandler("INVITE", new CmdInvite());
        dispatcher.registerHandler("KICK", new CmdKick());
        dispatcher.registerHandler("MODE", new CmdMode());
        inited = true;
    }
    return dispatcher;
}

/**
 * @brief Parsea una línea de comando enviada por un cliente y la ejecuta.
 *
 * Extrae el nombre del comando (texto hasta el primer espacio) y los
 * parámetros restantes. Registra el comando y delega la ejecución a
 * `executeCommand`.
 *
 * @param user Puntero al `User` que envió la línea.
 * @param commandLine Línea completa recibida del cliente (puede incluir CR/LF).
 */
void Server::handleClientCommand(User *user, const std::string &commandLine)
{
    if (!user) {
        std::cerr << "handleClientCommand: null user for command: " << commandLine << std::endl;
        return;
    }
    size_t sp = commandLine.find(' ');
    std::string cmd;
    std::string args;
    if (sp == std::string::npos) {
        cmd = commandLine;
        args = "";
    } else {
        cmd = commandLine.substr(0, sp);
        if (sp + 1 < commandLine.size())
            args = commandLine.substr(sp + 1);
        else
            args = "";
    }
    std::cout << "[" << cmd << "] " << "fd:" << user->getSocket() << " args:" << (args.empty() ? "<EMPTY>" : args) << std::endl;
    executeCommand(user, cmd, args);
}

/**
 * @brief Enruta y ejecuta el comando recibido según el estado de autenticación.
 *
 * - Si el `user` no está autenticado permite solo `PASS`, `NICK` y `USER`.
 *   Si tras esos valores el usuario queda completo, marca `authenticated`.
 * - Si está autenticado despacha los comandos conocidos a sus handlers
 *   correspondientes. En caso de comando no reconocido llama a
 *   `handleUnknownCommand`.
 *
 * @param user Usuario emisor del comando.
 * @param command Nombre del comando (esperado en mayúsculas).
 * @param args Parámetros asociados al comando.
 */
void Server::executeCommand(User *user, const std::string &command, const std::string &args)
{

    CommandDispatcher& dispatcher = getDispatcher();

    if (!user)
        return;


    if (!dispatcher.hasHandler(command))
    {
        handleUnknownCommand(user, command);
        return;
    }

    if (!user->isAuthenticated())
    {
        if (command != "PASS" && command != "NICK" && command != "USER" && command != "CAP" && command != "PING" && command != "PONG") {
            sendToUser(user, std::string("You're not authenticated"));
            return;
        }
    }

    RequestContext ctx(this->services, user, args);
    dispatcher.dispatch(command, ctx);
}

