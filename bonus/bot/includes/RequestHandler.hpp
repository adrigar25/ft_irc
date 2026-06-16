/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   RequestHandler.hpp								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:15:33 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 01:25:16 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "BotOptions.hpp"
#include <string>

class IRCConnection;
class ChannelManager;

class RequestHandler {
public:
	RequestHandler(IRCConnection *conn, ChannelManager *cm, const std::string &nick);
	~RequestHandler();

	void handleLine(const std::string &line);

private:
	IRCConnection *irc;
	ChannelManager *cm;
	std::string nick;

	const BotCmd &chooseReply(const std::string &text) const;
};

#endif
