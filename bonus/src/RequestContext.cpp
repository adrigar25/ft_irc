/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:43 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:46 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestContext.hpp"

RequestContext::RequestContext(Services &svc, User *s, const std::string &line): services(svc), sender(s), rawLine(line) {};
