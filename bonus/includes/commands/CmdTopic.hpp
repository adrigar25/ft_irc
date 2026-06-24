/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:29 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:30 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_TOPIC_HPP
#define CMD_TOPIC_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdTopic : public ICommand {
public:
	CmdTopic() {}
	virtual ~CmdTopic() {}
	virtual void execute(RequestContext &ctx);
};

#endif
