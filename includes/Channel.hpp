/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/05/26 19:46:13 by agarcia          ###   ########.fr       */
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
        std::string             topic;
        std::string             key;
        std::map<int, User*>    users;
        std::map<int, User*>    voiceUsers;
        std::map<int, User*>    operators;
        std::map<int, User*>    invitedUsers;
        std::map<int, User*>    bannedUsers;
        std::map<int, User*>    exceptedUsers;
        int                     userCount;
        bool                    isPrivate;
        bool                    keyRequired;
        int                     userLimit;
    public:
                Channel(std::string name, User *creator);
                ~Channel();
            void    addUser(User *user);
            void    deleteUser(User *user);
            void    changeRole(User *user, std::string role);
            void    addUserToMap(std::map<int, User*> &userMap, User *user);
            void    deleteUserFromMap(std::map<int, User*> &userMap, User *user);
            void    inviteUser(User *user);
            void    setIsPrivate(bool val);
            void    setUserLimit(int limit);
            void    setTopic(const std::string &t);
            bool    canUserJoin(User *user) const;
            bool    hasUser(User *user) const;
            bool    isUserOperator(User *user) const;
            bool    isUserVoice(User *user) const;
            bool    isUserInvited(User *user) const;
            bool    isUserBanned(User *user) const;
            bool    isUserExcepted(User *user) const;
            bool    isEmpty() const;
            int     getUserCount() const;
            int     getUserLimit() const;
            bool    getIsPrivate() const;
            bool    getKeyRequired() const;
            std::string                 getKey() const;
            const std::map<int, User*>& getUsers() const;
            std::string                 getName() const;
    /* Exceptions moved to includes/Exceptions.hpp */
};

#endif
