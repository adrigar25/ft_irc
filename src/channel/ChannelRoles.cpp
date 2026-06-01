#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>

/**
 * @brief Cambia el rol de un `user` añadiéndolo al conjunto correspondiente.
 *
 * Soporta roles: "operator", "voice", "invited", "banned", "excepted".
 * Lanza `roleNotFoundException` si el rol es inválido.
 */
void Channel::changeRole(User *user, std::string role)
{
    int fd = user->getSocket();
    if(this->users.find(fd) == this->users.end())
        throw IrcException(IRC_ERR_USER_NOT_FOUND, "User not found in channel");
    std::map<std::string, std::map<int, User*>*> roles;
    roles.insert(std::make_pair(std::string("operator"), &this->operators));
    roles.insert(std::make_pair(std::string("voice"), &this->voiceUsers));
    roles.insert(std::make_pair(std::string("invited"), &this->invitedUsers));
    roles.insert(std::make_pair(std::string("banned"), &this->bannedUsers));
    roles.insert(std::make_pair(std::string("excepted"), &this->exceptedUsers));

    if(roles.find(role) == roles.end())
        throw IrcException(IRC_ERR_ROLE_NOT_FOUND, "Role not found");
    roles[role]->insert(std::make_pair(fd, user));
}
