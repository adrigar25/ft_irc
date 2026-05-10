#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"
#include <map>
#include <string>
#include <exception>
#include <poll.h>
#include <memory>
#include <vector>

class Server
{
    private:
        int serverSocket;
        unsigned int port;
        std::string password;
        std::map<int, std::unique_ptr<User> > users;
        std::map<int, std::string> buffers;
        std::map<std::string, std::unique_ptr<Channel> > channels;
        std::vector<struct pollfd> fds;

        void handleEvents();
        void handleDisconnection(int idx);
        void handleNewConnection();
        void handleClientMessage(int fd, const std::string &message);
        void handleClientCommand(User *user, const std::string &command);
        void executeCommand(User *user, const std::string &command, const std::string &args);
        void sendToUser(User *user, const std::string &message);
        void addUser(std::unique_ptr<User> user);
        void removeUser(int fd);
        void createChannel(std::string name, User *creator);
        Channel* getChannel(const std::string &name);
    public:
        Server(unsigned int port, std::string password);
        ~Server();
        void startServer();
        void stopServer();
        int getPort() const;
        int getServerSocket() const;

    class userAlreadyExistsException : public std::exception
    {
    public:
        virtual const char* what() const noexcept { return "User already exists"; }
        virtual ~userAlreadyExistsException() noexcept {}
    };

    class channelAlreadyExistsException : public std::exception
    {
    public:
        virtual const char* what() const noexcept { return "Channel already exists"; }
        virtual ~channelAlreadyExistsException() noexcept {}
    };

    class userNotFoundException : public std::exception
    {
    public:
        virtual const char* what() const noexcept { return "User not found"; }
        virtual ~userNotFoundException() noexcept {}
    };

    class errorStartingServerException : public std::exception
    {
    public:
        int code;
        std::string message;
        errorStartingServerException(int code = 0, const std::string& msg = "Error starting server")
            : code(code), message(msg) {}
        virtual const char* what() const noexcept { return message.c_str(); }
        int getCode() const { return code; }
        virtual ~errorStartingServerException() noexcept {}
    };

    class errorAcceptingConnectionException : public std::exception
    {
    public:
        virtual const char* what() const noexcept { return "Error accepting connection"; }
        virtual ~errorAcceptingConnectionException() noexcept {}
    };
};

#endif