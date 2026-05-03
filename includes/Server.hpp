#ifndef SERVER_HPP
#define SERVER_HPP

    #include "User.hpp"
    #include "Channel.hpp"
    #include <map>
    #include <string>
    #include <exception>

    class Server
    {
        private:
            int serverSocket;
            unsigned int port;
            std::string password;
            std::map<std::string, User*> users;
            std::map<std::string, Channel*> channels;
            void addUser(User *user);
            void removeUser(User *user);
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
            virtual const char* what() const _NOEXCEPT
            {
                return "User already exists";
            }
            virtual ~userAlreadyExistsException() _NOEXCEPT {}
        };

        class channelAlreadyExistsException : public std::exception
        {
        public:
            virtual const char* what() const _NOEXCEPT
            {
                return "Channel already exists";
            }
            virtual ~channelAlreadyExistsException() _NOEXCEPT {}
        };

        class userNotFoundException : public std::exception
        {
        public:
            virtual const char* what() const _NOEXCEPT
            {
                return "User not found";
            }
            virtual ~userNotFoundException() _NOEXCEPT {}
        };

        class errorStartingServerException : public std::exception
        {
        public:
            int code;
            std::string message;
            errorStartingServerException(int code = 0, const std::string& msg = "Error starting server")
                : code(code), message(msg) {}
            virtual const char* what() const _NOEXCEPT
            {
                return message.c_str();
            }
            int getCode() const { return code; }
            virtual ~errorStartingServerException() _NOEXCEPT {}
        };
    };

#endif