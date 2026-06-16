/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:54 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 14:40:31 by agarcia          ###   ########.fr       */
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