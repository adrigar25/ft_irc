/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ICommand.hpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:36:09 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:36:11 by agarcia		  ###   ########.fr	   */
/*																			*/
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
