/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/05/15 00:08:09 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/15 16:38:25 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "User.hpp"
#include "Channel.hpp"
#include "Exceptions.hpp"
#include "Services.hpp"
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
		int							 serverSocket;
		unsigned int					port;
		std::string					 password;
		Services						services;
		std::string					 hostname;
		std::vector<struct pollfd>	  fds;
		static Server*				  instance;
		bool							running;

		void		handleEvents();
		int		 performPoll();
		void		handleClientEvents();
		void		processClientBuffer(User *user);
		bool		handleClientRead(int idx);
		User*	   getUserByFd(int fd);
		void		appendToUserInBuffer(User *user, const char *buffer, ssize_t n);
		bool		handleClientError(int idx);
		bool		handleClientWrite(int idx);
		void		handleDisconnectionByIndex(int idx);
		void		handleDisconnectionByFd(int fd);
		void		handleNewConnection();
		void		createServerSocket();
		void		setSocketOptions();
		void		bindServerSocket();
		void		listenServerSocket();
		void		setupPollFds();
		void		pushPollFd(int fd, short events = POLLIN);
		void		enablePollOutForFd(int fd);
		ssize_t	 sendNonBlockingOnce(int fd, const char *buf, size_t len);
		void		enqueuePending(User *user, const char *buf, size_t len);
		void		handleClientCommand(User *user, const std::string &commandLine);
		bool		handleUnknownCommand(User *user, const std::string &command);
		void		executeCommand(User *user, const std::string &command, const std::string &args);
		void		sendToUser(User *user, const std::string &message);
		void		sendToChannel(Channel *channel, const std::string &message, User *exclude);
		void		addUser(User* user);
		void		deleteUser(int fd);
		void		initSignals();
		void		cleanup();
	public:
				Server(unsigned int port, std::string password);
				~Server();
		void	sendMessageToUser(User *user, const std::string &message) { sendToUser(user, message); }
		void	sendMessageToChannel(Channel *channel, const std::string &message, User *exclude) { sendToChannel(channel, message, exclude); }
		void	startServer();
		void	stopServer();
		unsigned int getPort() const;
		const std::string& getPassword() const;
		int	 getServerSocket() const;
		bool	validatePassword(const std::string &p) const { return this->password == p; }
		const std::string& getHostname() const;
		int	 startBot();
		static Server* getInstance();

};

#endif
