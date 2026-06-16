/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdCap.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:34:53 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:34:53 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */



#ifndef CMD_CAP_HPP
#define CMD_CAP_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdCap : public ICommand {
public:
	CmdCap() {}
	virtual ~CmdCap() {}
	virtual void execute(RequestContext &ctx);
};

#endif
