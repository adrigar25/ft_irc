#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <map>

class Channel;

class User
{
    private:
        int socket;
        std::string nickname;
        std::string username;
        bool nickSet;
        bool userSet;
        bool passGiven;
        bool authenticated;
        std::map<std::string, Channel*> channels;
    public:
        User(int socket, const std::string &name);
        ~User();
        void joinChannel(Channel *channel);
        void leaveChannel(Channel *channel);
        std::string getNickname() const;
        int getSocket() const;
        const std::map<std::string, Channel*>& getChannels() const;
        
        // Authentication / identity helpers
        void setNickname(const std::string &name);
        void setUsername(const std::string &name);
        void setPass(bool val);
        void setAuthenticated(bool val);
        std::string getUsername() const;
        bool isPassSet() const;
        bool isNickSet() const;
        bool isUserSet() const;
        bool isAuthenticated() const;

        class userAlreadyInChannelException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "User already in channel";
            }
        };
};

#endif
