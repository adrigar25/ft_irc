/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:01:42 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:01:43 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "managers/UserManager.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Exceptions.hpp"
#include <sstream>

UserManager::UserManager() {}
UserManager::~UserManager() {}

void UserManager::add(User* user)
{
	if (!user) return;
	int fd = user->getSocket();
	if (users.find(fd) != users.end()) {
		std::ostringstream oss; oss << fd;
		throw IrcException(IRC_ERR_USER_ALREADY_EXISTS, std::string("User already exists: fd ") + oss.str());
	}
	users.insert(std::make_pair(fd, user));
}

void UserManager::remove(int fd)
{
	std::map<int, User*>::iterator it = users.find(fd);
	if (it == users.end()) return;
	User* user = it->second;
	users.erase(it);
	delete user;
}

User* UserManager::findByNick(const std::string &nick) const
{
	for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second->getNickname() == nick)
			return it->second;
	}
	return NULL;
}

User* UserManager::findByFd(int fd) const
{
	std::map<int, User*>::const_iterator it = users.find(fd);
	if (it == users.end()) return NULL;
	return it->second;
}

std::map<int, User*>& UserManager::getAll() { return users; }

void UserManager::clear()
{
	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); ++it)
		delete it->second;
	users.clear();
}

const std::map<int, User*>& UserManager::getAll() const { return users; }
