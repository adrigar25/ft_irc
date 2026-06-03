/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: GitHub Copilot                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by copilot           #+#    #+#             */
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
