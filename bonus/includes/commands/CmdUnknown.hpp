/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdUnknown.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:35:58 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:36:00 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef CMD_UNKNOWN_HPP
#define CMD_UNKNOWN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUnknown : public ICommand {
public:
	CmdUnknown() {}
	virtual ~CmdUnknown() {}
	virtual void execute(RequestContext &ctx);
};

#endif
