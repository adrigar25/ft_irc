/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:05:12 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/16 17:05:13 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>
#include "../RequestContext_bonus.hpp"

class ICommand {
public:
	virtual ~ICommand() {}
	virtual void execute(RequestContext &ctx) = 0;
};

#endif
