/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:29 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:04:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONTEXT_HPP
#define REQUESTCONTEXT_HPP

#include <string>
#include "User_bonus.hpp"

class Services;
struct RequestContext {
	Services &services;
	User *sender;
	std::string rawLine;

	RequestContext(Services &svc, User *s, const std::string &r);
};

#endif
