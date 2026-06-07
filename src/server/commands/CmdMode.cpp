/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:37:48 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 16:21:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CmdMode.hpp"
#include "RequestContext.hpp"
#include "Services.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>

static std::vector<std::string> splitLine(const std::string &line)
{
	std::vector<std::string> tokens;
	std::istringstream iss(line);
	std::string token;

	while(iss >> token)
		tokens.push_back(token);
	return (tokens);
}


static void handleModeI(Channel* ch, bool isAdding)
{
    ch->setIsInviteOnly(isAdding);
}

static void handleModeT(Channel* ch, bool isAdding)
{
    ch->setTopicProtected(isAdding);
}

static void handleModeK(Channel* ch, bool isAdding, const std::string &password)
{
    if (isAdding)
	{
        ch->setKey(password);
        ch->setKeyRequired(true);
    }
	else
	{
        ch->setKey("");
        ch->setKeyRequired(false);
    }
}

static bool handleModeO(Channel* ch, bool isAdding, User* targetUser, RequestContext &ctx)
{
    if (!targetUser)
        return true;
    
    if (isAdding)
        ch->changeRole(targetUser, "operator");
    else
	{
		if(targetUser->getNickname() == ctx.sender->getNickname())
			return true;
		if(ch->getOperators().size() == 1 && ch->isUserOperator(targetUser))
			return true;
        ch->removeRole(targetUser, "operator");
	}
	return false;
}

static void handleModeL(Channel* ch, bool isAdding, int limit,  RequestContext &ctx)
{
    if (isAdding)
	{
		if (limit < 1 || ch->getUserCount() > limit)
		{
			std::ostringstream oss;
			oss << "696 " << ctx.sender->getNickname() << " " << ch->getName() << " :Invalid limit";
			ctx.services.sendToUser(ctx.sender, oss.str());
			return ;
		}
		ch->setUserLimit(limit);
	}
    else
        ch->setUserLimit(-1);
}

static void handleModeB(Channel* ch, bool isAdding, User* targetUser)
{
    if(isAdding)
        ch->banUser(targetUser);
    else
		ch->unbanUser(targetUser);
}

static void handleModeV(Channel* ch, bool isAdding, User* targetUser)
{
    if (isAdding)
        ch->changeRole(targetUser, "voice");
    else
        ch->removeRole(targetUser, "voice");
}
static void handleModeM(Channel* ch, bool isAdding)
{
    ch->setModerated(isAdding);
}

static bool validateModeCommand(RequestContext &ctx, const std::vector<std::string> &tokens, Channel *&ch)
{
    if (tokens.size() < 2)
    {
        ctx.services.sendResponse(ctx, "461", "Not enough parameters");
        return false;
    }

    ch = ctx.services.channels().getChannel(tokens[0]);
    if (!ch)
    {
        ctx.services.sendResponse(ctx, "483", tokens[0] + " :No such channel");
        return false;
    }

    if (!ch->isUserOperator(ctx.sender))
    {
        ctx.services.sendResponse(ctx, "482", "You're not channel operator");
        return false;
    }
    return true;
}

static bool requireParameter(RequestContext &ctx, size_t paramIndex, const std::vector<std::string> &modeParams)
{
    if (paramIndex >= modeParams.size())
    {		
		ctx.services.sendResponse(ctx, "461", "Not enough parameters");
        return false;
    }
    return true;
}

static bool isValidLimit(const std::string &limitStr)
{
    for (size_t i = 0; i < limitStr.size(); ++i)
    {
        if (!std::isdigit(limitStr[i]))
            return false;
    }
    return !limitStr.empty();
}

static void broadcastMode(RequestContext &ctx, Channel *ch, const std::string &channelName, const std::string &modeString, const std::vector<std::string> &tokens)
{
    std::string modeMessage = std::string(":") + ctx.sender->getNickname() + " MODE " + channelName + " " + modeString;

    for (int i = 2; i < (int)tokens.size(); i++)
        modeMessage += " " + tokens[i];

    const std::map<int, User*>& users = ch->getUsers();
    for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it)
        ctx.services.sendToUser(it->second, modeMessage);
}

void CmdMode::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;
	if(ctx.rawLine.empty())
		return;
	
	std::vector<std::string> tokens = splitLine(ctx.rawLine);

	std::string channelName = tokens[0];
	std::string modeString = tokens[1];
	std::vector<std::string> modeParams;
	if(tokens.size() > 2)
		modeParams = std::vector<std::string>(tokens.begin() + 2, tokens.end());

	size_t paramIndex = 0;
	bool adding = true;
	bool errorOccurred = false;
	Channel* ch = NULL;
	if (!validateModeCommand(ctx, tokens, ch))
		return;

	for(size_t i = 0; i < modeString.size(); ++i)
	{
		char modeChar = modeString[i];
		if (modeChar == '+')
			adding = true;
		else if (modeChar == '-')
			adding = false;
		else
		{
			if (modeChar == 'i')
				handleModeI(ch, adding);
			else if (modeChar == 't')
				handleModeT(ch, adding);
			else if (modeChar == 'k')
			{
				if (!requireParameter(ctx, paramIndex, modeParams))
					return;
				handleModeK(ch, adding, modeParams[paramIndex++]);
			}
			else if (modeChar == 'o' || modeChar == 'v' || modeChar == 'b')
			{
				if (!requireParameter(ctx, paramIndex, modeParams))
					return;
				User* targetUser = ctx.services.users().findByNick(modeParams[paramIndex++]);
				if (!targetUser)
				{
					ctx.services.sendResponse(ctx, "401", modeParams[paramIndex - 1] + " :No such user");
					return ;
				}
				if (modeChar == 'o')
					errorOccurred = handleModeO(ch, adding, targetUser, ctx);
				else if (modeChar == 'v')
					handleModeV(ch, adding, targetUser);
				else if (modeChar == 'b')
					handleModeB(ch, adding, targetUser);
			}
			else if (modeChar == 'l')
			{
				if (adding)
				{
					if (!requireParameter(ctx, paramIndex, modeParams))
						return;

					std::string limitStr = modeParams[paramIndex];
					if (!isValidLimit(limitStr))
					{
						ctx.services.sendResponse(ctx, "696", ch->getName() + " :Invalid limit");
						return;
					}
					int limit = std::atoi(modeParams[paramIndex++].c_str());
					handleModeL(ch, adding, limit, ctx);
				}
				else
					handleModeL(ch, adding, -1, ctx);
			}
			else if (modeChar == 'm')
				handleModeM(ch, adding);
			else
			{
				ctx.services.sendResponse(ctx, "472", channelName + " :Unknown mode");
				return ;
			}
		}
	}

	if (!errorOccurred)
		broadcastMode(ctx, ch, channelName, modeString, tokens);
}
