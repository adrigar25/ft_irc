/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/09 18:18:42 by agarcia          ###   ########.fr       */
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
