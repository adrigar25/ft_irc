/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:22:33 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:33 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDDISPATCHER_HPP
#define COMMANDDISPATCHER_HPP

#include <map>
#include <string>

class ICommand;
struct RequestContext;

class CommandDispatcher {
public:
	CommandDispatcher();
	~CommandDispatcher();
	void registerHandler(const std::string& cmd, ICommand* handler);
	bool hasHandler(const std::string& cmd) const;
	void dispatch(const std::string& cmd, RequestContext &ctx) const;
private:
	std::map<std::string, ICommand*> handlers;
};

#endif
