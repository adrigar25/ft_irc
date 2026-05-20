/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:09 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/19 18:08:19 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "User.hpp"
#include "Channel.hpp"
#include <map>
#include <string>
#include <exception>
#include <stdexcept>
#include <poll.h>
#include <vector>

class Server
{
    private:
        int                             serverSocket;
        unsigned int                    port;
        std::string                     password;
        std::map<int, User*>            users;
        /* per-user buffers moved into User class */
        std::map<std::string, Channel*> channels;
        std::vector<struct pollfd>      fds;
    void        handleEvents();
    int         performPoll();
    void        handleClientEvents();
    void        processClientBuffer(User *user);
    bool        handleClientRead(int idx);
    User*       getUserByFd(int fd);
    void        appendToUserInBuffer(User *user, const char *buffer, ssize_t n);
    bool        handleClientError(int idx);
    bool        handleClientWrite(int idx);
    void        handleDisconnectionByIndex(int idx);
    void        handleDisconnectionByFd(int fd);
    void        handleNewConnection();
    void        createServerSocket();
    void        setSocketOptions();
    void        bindServerSocket();
    void        listenServerSocket();
    void        setupPollFds();
    void        pushPollFd(int fd, short events = POLLIN);
    void        enablePollOutForFd(int fd);
    ssize_t     sendNonBlockingOnce(int fd, const char *buf, size_t len);
    void        enqueuePending(User *user, const char *buf, size_t len);
    void        handleClientCommand(User *user, const std::string &commandLine);
    bool        handleUnknownCommand(User *user, const std::string &command);
    void        executeCommand(User *user, const std::string &command, const std::string &args);
    bool        handlePASS(User* user, const std::string& params);
    bool        handleNICK(User* user, const std::string& nick);
    bool        handleUSER(User* user, const std::string& params);
    bool        handleJOIN(User* user, const std::string& params);
    bool        handlePART(User* user, const std::string& params);
    bool        handlePRIVMSG(User* user, const std::string& params);
    bool        handleQUIT(User* user, const std::string& params);
    bool        handleLIST(User* user, const std::string& params);
    bool        handleINVITE(User* user, const std::string& params);
    bool        handleKICK(User* user, const std::string& params);
    bool        handleMODE(User* user, const std::string& params);
    void        sendToUser(User *user, const std::string &message);
    void        sendToChannel(Channel *channel, const std::string &message, User *exclude);
    void        addUser(User* user);
    void        deleteUser(int fd);
    void        createChannel(std::string name, User *creator);
    Channel*    getChannel(const std::string &name);
    public:
        Server(unsigned int port, std::string password);
        ~Server();
    void    startServer();
    void    stopServer();
    int     getPort() const;
    int     getServerSocket() const;
    User    *getUserByNickname(const std::string &nickname);

    class userAlreadyExistsException : public std::runtime_error
    {
        public:
            explicit userAlreadyExistsException(const std::string &msg = "User already exists")
                : std::runtime_error(msg) {}
            virtual ~userAlreadyExistsException() noexcept {}
    };

    class channelAlreadyExistsException : public std::runtime_error
    {
        public:
            explicit channelAlreadyExistsException(const std::string &msg = "Channel already exists")
                : std::runtime_error(msg) {}
            virtual ~channelAlreadyExistsException() noexcept {}
    };

    class userNotFoundException : public std::runtime_error
    {
        public:
            explicit userNotFoundException(const std::string &msg = "User not found")
                : std::runtime_error(msg) {}
            virtual ~userNotFoundException() noexcept {}
    };

    class errorStartingServerException : public std::runtime_error
    {
        public:
            int code;
            explicit errorStartingServerException(int code = 0, const std::string& msg = "Error starting server")
                : std::runtime_error(msg), code(code) {}
            int getCode() const { return code; }
            virtual ~errorStartingServerException() noexcept {}
    };

    class errorAcceptingConnectionException : public std::runtime_error
    {
        public:
            explicit errorAcceptingConnectionException(const std::string &msg = "Error accepting connection")
                : std::runtime_error(msg) {}
            virtual ~errorAcceptingConnectionException() noexcept {}
    };

    class errorSettingNonblockingException : public std::runtime_error
    {
        public:
            explicit errorSettingNonblockingException(const std::string &msg = "Error setting non-blocking mode")
                : std::runtime_error(msg) {}
            virtual ~errorSettingNonblockingException() noexcept {}
    };

    class errorSettingCloexecException : public std::runtime_error
    {
        public:
            explicit errorSettingCloexecException(const std::string &msg = "Error setting close-on-exec flag")
                : std::runtime_error(msg) {}
            virtual ~errorSettingCloexecException() noexcept {}
    };
};

#endif