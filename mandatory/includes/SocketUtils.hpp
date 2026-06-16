/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:14 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:04:15 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

void setSocketNonBlocking(int fd);
void setSocketCloexec(int fd);
std::string formatMessage(const std::string &message);

#endif
