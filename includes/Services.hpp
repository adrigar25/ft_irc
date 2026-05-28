/* ************************************************************************** */
/*                                                                            */
/*                                                     Services.hpp            */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <string>
#include "managers/UserManager.hpp"
#include "managers/ChannelManager.hpp"

class Server;
class User;

class Services {
public:
    Services(Server* srv);
    ~Services();

    Server* getServer() const;

private:
    Server* server;
    UserManager userManager;
    ChannelManager channelManager;
public:
    UserManager& users() { return userManager; }
    ChannelManager& channels() { return channelManager; }
    const UserManager& users() const { return userManager; }
    const ChannelManager& channels() const { return channelManager; }
     /* Helpers to send messages through the server.
         These delegate to `Server::sendMessageToUser/Channel` and exist so
         command handlers can send messages without touching sockets or
         poll internals. They are thin shims and can be removed if callers
         use the Server API directly, but keeping them improves testability
         and decoupling. */
     void sendToUser(User* user, const std::string &message);
     void sendToChannel(Channel* channel, const std::string &message, User* exclude = NULL);
};

#endif
