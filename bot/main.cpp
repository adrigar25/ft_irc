/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 20:17:22 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/08 14:22:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include "Bot.hpp"

static std::string read_port_file(const std::string &path, int tries=50, int wait_ms=200){
	for(int i=0;i<tries;i++){
		std::ifstream f(path);
		if (f){
			std::string p; std::getline(f, p);
			if (!p.empty()) return p;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
	}
	return "";
}

static int determine_port(int argc, char **argv){
	int port = 0;
	if (argc > 3){
		std::string p = argv[3];
		if (p == "auto"){
			std::string s = read_port_file(".irc_port");
			if (s.empty())
				port = std::atoi(BOT_DEFAULT_PORT);
			else
				port = std::atoi(s.c_str());
		} else {
			port = std::atoi(argv[3]);
		}
	} else {
		std::string s = read_port_file(".irc_port");
		if (s.empty())
			port = std::atoi(BOT_DEFAULT_PORT);
		else
			port = std::atoi(s.c_str());
	}
	return port;
}

int main(int argc, char **argv){

	std::string nick = BOT_DEFAULT_NICK;
	std::string host = BOT_DEFAULT_HOST;
	std::string password = "";

	if (argc > 1 && std::string(argv[1]) == "--help"){
		std::cout << "Usage: " << argv[0] << " [nick] [host] [port] [password]\n";
		std::cout << "If host is 'auto', it will connect to localhost. If port is 'auto', it will read the port from .irc_port.\n";
		return 0;
	}

	if (argc > 1)
		nick = argv[1];

	if (argc > 2)
		host = argv[2];

	if (argc > 4)
		password = argv[4];

	Bot bot(nick, host, password);
	int port = determine_port(argc, argv);
	bot.setPort(port);
	if (bot.connectToServer() == -1){
		std::cerr << "Failed to connect to " << bot.getHost() << ":" << bot.port << "\n";
		return 1;
	}
	bot.run();
	
	return 0;
}
