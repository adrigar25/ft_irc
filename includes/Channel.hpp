/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:03 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/01 20:37:11 by agarcia          ###   ########.fr       */
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
        bool                    topicProtected;
        bool                    keyRequired;
        bool                    isInviteOnly;
        bool                    isSecret;
        int                     userLimit;
    public:
                Channel(std::string name, User *creator);
                ~Channel();

            /* Gestión de usuarios */
            void    addUser(User *user);
            void    deleteUser(User *user);
            void    addUserToMap(std::map<int, User*> &userMap, User *user);
            void    deleteUserFromMap(std::map<int, User*> &userMap, User *user);
            void    inviteUser(User *user);
            void    banUser(User *user);
            void    unbanUser(User *user);

            /* Roles */
            void    changeRole(User *user, std::string role);
			void	removeRole(User *user, std::string role);

            /* Setters */
            void    setIsInviteOnly(bool val);
            void    setUserLimit(int limit);
            void    setTopic(const std::string &t);
            void    setKey(const std::string &k);
            void    setKeyRequired(bool val);
            void    setIsSecret(bool val);
            void    setTopicProtected(bool val);

            /* Consultas */
            bool    canUserJoin(User *user) const;
            bool    hasUser(User *user) const;
            bool    isUserOperator(User *user) const;
            bool    isUserVoice(User *user) const;
            bool    isUserInvited(User *user) const;
            bool    isUserBanned(User *user) const;
            bool    isUserExcepted(User *user) const;
            bool    isEmpty() const;

            /* Getters */
            int     getUserCount() const;
            int     getUserLimit() const;
            bool    getIsInviteOnly() const;
            bool    getTopicProtected() const;
            bool    getIsSecret() const;
            bool    getKeyRequired() const;

            std::string                 getTopic() const;
            std::string                 getKey() const;
            const std::map<int, User*>& getUsers() const;
            const std::map<int, User*>& getOperators() const;
            const std::map<int, User*>& getVoiceUsers() const;
            const std::map<int, User*>& getInvitedUsers() const;
            const std::map<int, User*>& getBannedUsers() const;
            const std::map<int, User*>& getExceptedUsers() const;
            std::string                 getName() const;
    /* Exceptions moved to includes/Exceptions.hpp */
};

#endif
