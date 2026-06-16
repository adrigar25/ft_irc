/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:21 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:21 by adriescr         ###   ########.fr       */
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
