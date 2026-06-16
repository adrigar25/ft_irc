/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdUnknown.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:38:09 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/07 12:38:10 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "commands/CmdUnknown.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>
#include "replies/Replies.hpp"

void CmdUnknown::execute(RequestContext &ctx)
{
	if (!ctx.sender) return;
	if (ctx.rawLine.empty())
		return;
	ctx.services.sendResponse(ctx, ERR_UNKNOWNCOMMAND(ctx.sender->getNickname(), ctx.rawLine));
}
#include "Server.hpp"
#include "User.hpp"
#include <string>

bool Server::handleUnknownCommand(User *user, const std::string &command)
{
	if (command.empty())
		return false;
	std::string reply = ERR_UNKNOWNCOMMAND(user->getNickname(), command);
	sendToUser(user, std::string(":") + this->services.getServerName() + " " + reply);
	return true;
}
