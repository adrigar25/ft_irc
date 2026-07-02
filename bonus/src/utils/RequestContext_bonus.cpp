/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:58:18 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 16:58:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestContext_bonus.hpp"


RequestContext::RequestContext(Services &svc, User *s, const std::string &line): services(svc), sender(s), rawLine(line) {};
