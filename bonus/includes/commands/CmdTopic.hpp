/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:36 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:44 by adriescr         ###   ########.fr       */
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
