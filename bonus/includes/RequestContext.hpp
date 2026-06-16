/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:44 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:17:12 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONTEXT_HPP
#define REQUESTCONTEXT_HPP

#include <string>
#include "User.hpp"

class Services;
struct RequestContext {
	Services &services;
	User *sender;
	std::string rawLine;

	RequestContext(Services &svc, User *s, const std::string &r);
};

#endif
