/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:46 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 17:59:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>

class User;

class Channel
{
	private:
		std::string name;
		std::string topic;
		std::string key;
		std::map<int, User*> users;
		std::map<int, User*> voiceUsers;
		std::map<int, User*> operators;
		std::map<int, User*> invitedUsers;
		std::map<int, User*> bannedUsers;
		int userCount;
		bool topicProtected;
		bool keyRequired;
		bool isInviteOnly;
		bool isSecret;
		bool isModerated;
		int userLimit;
	public:
		Channel(std::string name);
		~Channel();

		/* Gestión de usuarios */
		void addUser(User *user);
		void deleteUser(const User *user);
		void addUserToMap(std::map<int, User*> &userMap, User *user);
		void deleteUserFromMap(std::map<int, User *> &userMap, const User *user);
		void inviteUser(User *user);
		void removeInvitedUser(User *user);
		void banUser(User *user);
		void unbanUser(User *user);

		/* Roles */
		void changeRole(User *user, std::string role);
		void removeRole(const User *user, std::string role);

		/* Setters */
		void setIsInviteOnly(bool val);
		void setUserLimit(int limit);
		void setTopic(const std::string &t);
		void setKey(const std::string &k);
		void setKeyRequired(bool val);
		void setIsSecret(bool val);
		void setTopicProtected(bool val);
		void setModerated(bool val);

		/* Consultas */
		void canUserJoin(User *user, const std::string &key) const;
		bool hasUser(const User *user) const;
		bool isUserOperator(User *user) const;
		bool isUserVoice(const User *user) const;
		bool isUserInvited(const User *user) const;
		bool isUserBanned(const User *user) const;
		bool isEmpty() const;

		/* Getters */
		int	getUserCount() const;
		int	getUserLimit() const;
		bool getIsInviteOnly() const;
		bool getTopicProtected() const;
		bool getIsSecret() const;
		bool getKeyRequired() const;
		bool getIsModerated() const;
		void getChannelModes(std::string &modes, std::vector<std::string> &params) const;

		std::string getTopic() const;
		std::string	getKey() const;
		std::map<int, User*>& getUsers();
		std::map<int, User*>& getOperators();
		std::map<int, User*>& getVoiceUsers();
		std::map<int, User*>& getInvitedUsers();
		std::map<int, User*>& getBannedUsers();
		std::map<int, User*>& getExceptedUsers();
		std::string	getName() const;
};

#endif
