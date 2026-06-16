/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdQuit.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:35:44 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:35:46 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CMD_QUIT_HPP
#define CMD_QUIT_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdQuit : public ICommand {
public:
	CmdQuit() {}
	virtual ~CmdQuit() {}
	virtual void execute(RequestContext &ctx);
};

#endif
