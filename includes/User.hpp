#ifndef USER_HPP
#define USER_HPP

#include <string>

class Channel;

class User
{
    private:
        std::string name;
        Channel *currentChannel;
    public:
        User(std::string name);
        ~User();
        void joinChannel(Channel *channel);
        std::string getNickname() const;
};

#endif
