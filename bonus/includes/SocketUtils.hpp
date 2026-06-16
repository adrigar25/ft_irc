/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   SocketUtils.hpp									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:37:06 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:37:07 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

void setSocketNonBlocking(int fd);
void setSocketCloexec(int fd);
std::string formatMessage(const std::string &message);

#endif
