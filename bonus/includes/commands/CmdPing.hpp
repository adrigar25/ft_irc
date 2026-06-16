/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdPing.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:35:36 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:35:36 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */



#ifndef CMD_PING_HPP
#define CMD_PING_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPing : public ICommand {
public:
	CmdPing() {}
	virtual ~CmdPing() {}
	virtual void execute(RequestContext &ctx);
};

#endif
