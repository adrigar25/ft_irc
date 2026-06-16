/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdUser.hpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:36:04 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:36:05 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CMD_USER_HPP
#define CMD_USER_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUser : public ICommand {
public:
	CmdUser() {}
	virtual ~CmdUser() {}
	virtual void execute(RequestContext &ctx);
};

#endif
