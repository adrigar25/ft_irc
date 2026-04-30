#ifndef SERVER_HPP
#define SERVER_HPP

    #include "User.hpp"
    #include "Channel.hpp"
    #include <map>
    #include <string>

    class Server
    {
        private:
            unsigned int port;
            std::string password;
            std::map<std::string, User*> users;
            std::map<std::string, Channel*> channels;
        public:
            Server(unsigned int port, std::string password);
            ~Server();
            void startServer();
            void stopServer();
            void addUser(User *user);
            void removeUser(User *user);
            void createChannel(std::string name, User *creator);
            Channel* getChannel(const std::string &name);
        
        class userAlreadyExistsException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "User already exists";
            }
        };

        class channelAlreadyExistsException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "Channel already exists";
            }
        };

        class userNotFoundException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "User not found";
            }
        };
    };

#endif