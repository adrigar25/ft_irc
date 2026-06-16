/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdNick.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:35:21 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:35:21 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */



#ifndef CMD_NICK_HPP
#define CMD_NICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdNick : public ICommand {
public:
	CmdNick() {}
	virtual ~CmdNick() {}
	virtual void execute(RequestContext &ctx);
};

#endif
