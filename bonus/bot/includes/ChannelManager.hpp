/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ChannelManager.hpp								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:16:02 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 01:56:03 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CHANNEL_MANAGER_HPP
#define CHANNEL_MANAGER_HPP

#include <string>
#include <vector>

class IRCConnection;

class ChannelManager {
public:
	ChannelManager(IRCConnection *conn);
	~ChannelManager();

	bool checkUserOP(const std::string &channel, const std::string &user);
	void deleteChannel(const std::string &channel);
	void joinChannel(const std::string &channel);
	void partChannel(const std::string &channel);
	bool isInChannel(const std::string &channel) const;

private:
	IRCConnection *conn;
	std::vector<std::string> joined;
};

#endif
