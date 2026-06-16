/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:10:49 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:15:32 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>
#include "../RequestContext.hpp"

class ICommand {
	public:
		virtual ~ICommand() {}
		virtual void execute(RequestContext &ctx) = 0;
};

#endif
