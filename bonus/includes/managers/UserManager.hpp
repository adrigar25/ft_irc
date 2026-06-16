/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:07 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:25 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <string>
#include <map>

class User;

class Server;
class User;

class UserManager {
	public:
		UserManager();
		~UserManager();

		void add(User* user);
		void remove(int fd);
		User* findByNick(const std::string &nick) const;
		User* findByFd(int fd) const;
		std::map<int, User*>& getAll();
		const std::map<int, User*>& getAll() const;
		void clear();

	private:
		std::map<int, User*> users;
};

#endif
