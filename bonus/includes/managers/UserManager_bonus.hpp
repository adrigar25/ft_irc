/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:04:58 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/24 13:59:18 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <string>
#include <map>

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
