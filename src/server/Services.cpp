#include "Services.hpp"
#include "Server.hpp"

Services::Services(Server* srv): server(srv), userManager(), channelManager() {}

Services::~Services() {}

Server* Services::getServer() const { return server; }

/**
 * @brief Envía `message` a `user` delegando la lógica de I/O al servidor.
 *
 * @param user Puntero al usuario destinatario.
 * @param message Mensaje a enviar.
 */
void Services::sendToUser(User* user, const std::string &message) {
	if (server) server->sendMessageToUser(user, message);
}

/**
 * @brief Envía `message` a los usuarios de `channel`, excluyendo `exclude`.
 *
 * @param channel Canal destino.
 * @param message Mensaje a enviar.
 * @param exclude Usuario a excluir (opcional).
 */
void Services::sendToChannel(Channel* channel, const std::string &message, User* exclude) {
	if (server) server->sendMessageToChannel(channel, message, exclude);
}
