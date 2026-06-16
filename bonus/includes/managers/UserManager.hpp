/* ************************************************************************** */
/*																			*/
/*												  UserManager.hpp		   */
/*																			*/
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
