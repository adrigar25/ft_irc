/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/15 00:53:28 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>

class User;

/**
 * @brief Representa un canal IRC y gestiona usuarios y roles.
 */
class Channel
{
    private:
        std::string             name;
        std::map<int, User*>    users;
        std::map<int, User*>    voiceUsers;
        std::map<int, User*>    operators;
        std::map<int, User*>    invitedUsers;
        std::map<int, User*>    bannedUsers;
        std::map<int, User*>    exceptedUsers;
        int                     userCount;
        bool                    isPrivate;
        int                     userLimit;
    public:
                Channel(std::string name, User *creator);
                ~Channel();
            void    addUser(User *user);
            void    deleteUser(User *user);
            void    changeRole(User *user, std::string role);
            void    addUserToMap(std::map<int, User*> &userMap, User *user);
            void    deleteUserFromMap(std::map<int, User*> &userMap, User *user);
            bool    canUserJoin(User *user) const;
            bool    hasUser(User *user) const;
            bool    isUserOperator(User *user) const;
            bool    isUserVoice(User *user) const;
            bool    isUserInvited(User *user) const;
            bool    isUserBanned(User *user) const;
            bool    isUserExcepted(User *user) const;
            int     getUserCount() const;
            int     getUserLimit() const;
            bool    getIsPrivate() const;
            const std::map<int, User*>& getUsers() const;
            std::string                 getName() const;
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
    class userBannedException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "User is banned from this channel";
        }
    };
};

#endif
