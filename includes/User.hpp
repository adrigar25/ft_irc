#ifndef USER_HPP
#define USER_HPP

#include <string>

class Channel;

class User
{
    private:
        int socket;
        std::string nickname;
        std::string username;
        Channel *currentChannel;
        bool nickSet;
        bool userSet;
        bool passGiven;
        bool authenticated;
    public:
        User(int socket, const std::string &name);
        ~User();
        void joinChannel(Channel *channel);
        void setCurrentChannel(Channel *channel);
        Channel* getCurrentChannel() const;
        std::string getNickname() const;
        int getSocket() const;
        
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
};

#endif
