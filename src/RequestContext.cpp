/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestContext.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:35 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/09 17:34:09 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RequestContext.hpp"


RequestContext::RequestContext(Services &svc, User *s, const std::string &line): services(svc), sender(s), rawLine(line) {};
