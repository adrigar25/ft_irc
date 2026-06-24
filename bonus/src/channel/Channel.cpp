/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:58 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:03:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <map>

/**
 * @brief Constructor de `Channel`.
 *
 * Crea un canal con el nombre `name`, añade al `creator` como usuario
 * y lo marca como operador.
 * @param name Nombre del canal.
 * @param creator Usuario que crea el canal.
 */
Channel::Channel(std::string name, User *creator)
: name(name), topic(), key(), users(), voiceUsers(), operators(), invitedUsers(), bannedUsers(), userCount(0), topicProtected(false), keyRequired(false), isInviteOnly(false), isSecret(false), userLimit(-1)
{
	(void)creator;
}


/**
 * @brief Destructor de `Channel`.
 */
Channel::~Channel()
{
};
