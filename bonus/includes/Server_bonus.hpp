/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:52 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/29 18:18:13 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "User_bonus.hpp"
#include "Channel_bonus.hpp"
#include "Exceptions_bonus.hpp"
#include "Services_bonus.hpp"
#include <map>
#include <string>
#include <exception>
#include <stdexcept>
#include <poll.h>
#include <vector>
#include <sys/types.h>
#include <cstring>

#define DEFAULT_PORT 6667

class Server
{
	private:
		int serverSocket;
		unsigned int port;
		std::string password;
		Services services;
		std::string hostname;
		std::vector<struct pollfd> fds;
		static Server *instance;
		bool running;

		void handleEvents();
		int performPoll();
		void handleClientEvents();
		void processClientBuffer(User *user);
		bool handleClientRead(int idx);
		User *getUserByFd(int fd);
		void appendToUserInBuffer(User *user, const char *buffer, ssize_t n);
		bool handleClientError(int idx);
		bool handleClientWrite(int idx);
		void handleDisconnectionByIndex(int idx);
		void DisconnectUser(User *user);
		void handleNewConnection();
		void createServerSocket();
		void setSocketOptions();
		void bindServerSocket();
		void listenServerSocket();
		void setupPollFds();
		void pushPollFd(int fd, short events = POLLIN);
		void enablePollOutForFd(int fd);
		ssize_t sendNonBlockingOnce(int fd, const char *buf, size_t len);
		void enqueuePending(User *user, const char *buf, size_t len);
		void handleClientCommand(User *user, const std::string &commandLine);
		bool handleUnknownCommand(User *user, const std::string &command);
		void executeCommand(User *user, const std::string &command, const std::string &args);
		void addUser(User *user);
		void deleteUser(int fd);
		void initSignals();
		void cleanup();

	public:
		Server(unsigned int port, std::string password);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
		void sendToUser(User *user, const std::string &message);
		void sendToChannel(Channel *channel, const std::string &message, const User *exclude);
		void startServer();
		void stopServer();
		int startBot();
		unsigned int getPort() const;
		const std::string &getPassword() const;
		int getServerSocket() const;
		Services &getServices();
		bool validatePassword(const std::string &p) const { return this->password == p; }
		const std::string &getHostname() const;
		static Server *getInstance();
		void handleDisconnectionByFd(int fd);
};

#endif
