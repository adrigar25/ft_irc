/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   IrcConnection.hpp								  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:15:52 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 00:49:12 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef IRCCONNECTION_HPP
#define IRCCONNECTION_HPP

#include <string>

class IRCConnection {
public:
	IRCConnection();
	~IRCConnection();

	int connectTo(const std::string &host, int port);
	ssize_t sendRaw(const std::string &s);
	std::string recvLine();
	void closeConn();
	bool isConnected() const;
	int getFd() const;

private:
	int sockfd;
};

#endif

