/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:03:58 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/28 18:39:47 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel_bonus.hpp"
#include "User_bonus.hpp"
#include "Exceptions_bonus.hpp"
#include <map>

/**
 * @brief Constructor de `Channel`.
 *
 * Crea un canal con el nombre `name`, añade al `creator` como usuario
 * y lo marca como operador.
 * @param name Nombre del canal.
 * @param creator Usuario que crea el canal.
 */
Channel::Channel(const std::string &name) : 
	name(name),
	userCount(0),
	topicProtected(false),
	keyRequired(false),
	isInviteOnly(false),
	isSecret(false),
	isModerated(false),
	userLimit(-1) {}

/**
 * @brief Destructor de `Channel`.
 */
Channel::~Channel() {};
