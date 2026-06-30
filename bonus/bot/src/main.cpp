/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 20:17:22 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/15 16:03:36 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include "Bot.hpp"

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <password> <bot_nick>\n";
		return 1;
	}

	int port;
	std::string host = argv[1];
	std::string password = argv[3];
	std::string nick = argv[4];
	std::stringstream ss(argv[2]);
	ss >> port;

	Bot bot(host, port, password, nick);

	if (bot.connectToServer() == -1)
	{
		std::cerr << "Failed to connect to " << host << ":" << port << std::endl;
		return 1;
	}

	bot.run();
	return 0;
}
