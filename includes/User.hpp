#ifndef USER_HPP
#define USER_HPP

#include <string>

class Channel;

class User
{
    private:
        int socket;
        std::string nickname;
        Channel *currentChannel;
    public:
        User(int socket, const std::string &name);
        ~User();
        void joinChannel(Channel *channel);
        std::string getNickname() const;
};

#endif
