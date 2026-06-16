/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   CmdTopic.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/07 12:38:07 by agarcia		   #+#	#+#			 */
/*   Updated: 2026/06/09 17:02:10 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "commands/CmdTopic.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include "User.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "LineUtils.hpp"
#include "replies/Replies.hpp"

static void parseTopicParams(const std::string &params, std::string &outChannelName, std::string &outTopic)
{
	size_t sp = params.find(' ');
	outChannelName = (sp == std::string::npos) ? params : params.substr(0, sp);
	outTopic = (sp == std::string::npos) ? "" : params.substr(sp + 1);
	if (!outTopic.empty() && outTopic[0] == ':') outTopic.erase(0,1);
}

void CmdTopic::execute(RequestContext &ctx)
{

	std::string channelName;
	std::string topic;
	std::string params;

	if (!ctx.sender)
			return;

	params = trim(ctx.rawLine, " \r");

	std::string serverName = ctx.services.getServerName();
	std::string uname = ctx.sender->getUsername();
	if (uname.empty())
		uname = "~";

	parseTopicParams(params, channelName, topic);

	if(channelName.empty()) {
		ctx.services.sendResponse(ctx, ERR_NEEDMOREPARAMS(ctx.sender->getNickname(), "TOPIC"));
		return;
	}

	Channel *targetChannel = ctx.services.channels().getChannel(channelName);

	if(!targetChannel) {
		ctx.services.sendResponse(ctx, ERR_NOSUCHCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}
	if(topic.empty()) {
		if(targetChannel->getTopic().empty())
			ctx.services.sendResponse(ctx, RPL_NOTOPIC(ctx.sender->getNickname(), channelName));
		else
			ctx.services.sendResponse(ctx, RPL_TOPIC(ctx.sender->getNickname(), channelName, targetChannel->getTopic()));
		return;
	}
	if(!targetChannel->hasUser(ctx.sender)) {
		ctx.services.sendResponse(ctx, ERR_NOTONCHANNEL(ctx.sender->getNickname(), channelName));
		return;
	}
	if(!targetChannel->isUserOperator(ctx.sender) && targetChannel->getTopicProtected()) {
		ctx.services.sendResponse(ctx, ERR_CHANOPRIVSNEEDED(ctx.sender->getNickname(), channelName));
		return;
	}
	targetChannel->setTopic(topic);
	std::string response = ":" + ctx.sender->getNickname() + "!" + uname + "@" + serverName + " TOPIC " + channelName + " :" + topic;
	ctx.services.sendToChannel(targetChannel, response);
}
