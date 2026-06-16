/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:12:13 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:12:14 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

void setSocketNonBlocking(int fd);
void setSocketCloexec(int fd);
std::string formatMessage(const std::string &message);

#endif
