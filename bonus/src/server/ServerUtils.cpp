/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:21 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/22 10:48:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const std::string& Server::getHostname() const
{
	return this->hostname;
}

unsigned int Server::getPort() const
{
	return this->port;
}

const std::string& Server::getPassword() const
{
	return this->password;
}

Services &Server::getServices()
{
	return this->services;
}
