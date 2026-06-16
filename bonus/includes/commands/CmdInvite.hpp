/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdInvite.hpp									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:34:57 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:34:57 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */



#ifndef CMD_INVITE_HPP
#define CMD_INVITE_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdInvite : public ICommand {
public:
	CmdInvite() {}
	virtual ~CmdInvite() {}
	virtual void execute(RequestContext &ctx);
};

#endif
