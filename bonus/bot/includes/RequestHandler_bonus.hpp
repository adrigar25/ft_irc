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

#include "BotOptions_bonus.hpp"
#include <vector>
#include <string>

class IRCConnection;
class ChannelManager;

enum e_command
{
	CMD_PING,
	CMD_INVITE,
	CMD_KICK,
	CMD_PART,
	CMD_QUIT,
	CMD_PRIVMSG,
	CMD_UNKNOWN
};

struct Message
{
	std::string user;
	std::string cmd;
	std::vector<std::string> params;
	std::string trailing;
};

class RequestHandler {
	public:
		RequestHandler(IRCConnection *irc, ChannelManager *cm, const std::string &nick);
		~RequestHandler();

		void handleLine(const std::string &line);
		void handlePRIVMSG(const std::string &user, const std::string &channel, const std::string &msg);

		private : IRCConnection *irc;
		ChannelManager *cm;
		std::string nick;

		const BotCmd &chooseReply(const std::string &text) const;
};

#endif
