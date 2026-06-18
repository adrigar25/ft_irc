/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:12:13 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/19 01:01:57 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

void setSocketNonBlocking(int fd);
std::string formatMessage(const std::string &message);

#endif
