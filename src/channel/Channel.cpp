#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>
#include <iostream>

/**
 * @brief Constructor de `Channel`.
 *
 * Crea un canal con el nombre `name`, añade al `creator` como usuario
 * y lo marca como operador.
 * @param name Nombre del canal.
 * @param creator Usuario que crea el canal.
 */
Channel::Channel(std::string name, User *creator)
: name(name), topic(), key(), users(), voiceUsers(), operators(), invitedUsers(), bannedUsers(), exceptedUsers(), userCount(0), topicProtected(false), keyRequired(false), isInviteOnly(false), isSecret(false), userLimit(-1)
{
    if (creator) {
        this->addUser(creator);
        this->changeRole(creator, "operator");
    }
    if (creator)
        std::cout << "Channel " << this->name << " created by " << creator->getNickname() << std::endl;
    else
        std::cout << "Channel " << this->name << " created" << std::endl;
    
}


/**
 * @brief Destructor de `Channel`.
 */
Channel::~Channel()
{
    std::cout << "Channel " << this->name << " deleted" << std::endl;
};

