#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>

class User;

class Channel
{
    private:
        std::string name;
        std::map<std::string, User*> users;
        std::map<std::string, User*> voiceUsers;
        std::map<std::string, User*> operators;
        std::map<std::string, User*> invitedUsers;
        std::map<std::string, User*> bannedUsers;
        std::map<std::string, User*> exceptedUsers;
    int userCount;
    bool isPrivate;
    //bool isModerated;
    int userLimit;
    public:
        Channel(std::string name, User *creator);
        ~Channel();
        void addUser(User *user);
        void removeUser(User *user);
        void changeRole(User *user, std::string role);
        void addUserToMap(std::map<std::string, User*> &userMap, User *user);
        void removeUserFromMap(std::map<std::string, User*> &userMap, User *user);
    class userAlreadyExistsException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "User already exists in channel";
        }
    };
    class userNotFoundException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "User not found in channel";
        }
    };
    class channelFullException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Channel is full";
        }
    };
    class roleNotFoundException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Role not found";
        }
    };
    class privateChannelException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Channel is private, user must be invited to join";
        }
    };
};

#endif
