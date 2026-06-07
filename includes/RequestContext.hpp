/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:36:36 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONTEXT_HPP
#define REQUESTCONTEXT_HPP

#include <string>
#include "Services.hpp"
#include "User.hpp"

struct RequestContext {
    Services &services;
    User *sender;
    std::string rawLine;

    RequestContext(Services &svc, User *s, const std::string &r): services(svc), sender(s), rawLine(r) {}
};

#endif
