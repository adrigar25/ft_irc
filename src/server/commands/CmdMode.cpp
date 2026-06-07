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

static void handleModeO(Channel* ch, bool isAdding, User* targetUser)
{
    if (!targetUser)
	{
        return;
    }
    
    if (isAdding)
        ch->changeRole(targetUser, "operator");
    else
        ch->removeRole(targetUser, "operator");
}

static void handleModeL(Channel* ch, bool isAdding, int limit)
{
    if (isAdding)
        ch->setUserLimit(limit);
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

void CmdMode::execute(RequestContext &ctx)
{
	if (!ctx.sender)
		return;
	if(ctx.rawLine.empty())
		return;
	
	std::vector<std::string> tokens = splitLine(ctx.rawLine);

	if (tokens.size() < 2)
	{
		ctx.services.sendToUser(ctx.sender, "Need channel and mode");
		return ;
	}

	std::string channelName = tokens[0];
	std::string modeString = tokens[1];

	Channel* ch = ctx.services.channels().getChannel(channelName);
	if (!ch)
	{
		ctx.services.sendToUser(ctx.sender, "No such channel");
		return ;
	}

	if (!ch->isUserOperator(ctx.sender))
	{
		ctx.services.sendToUser(ctx.sender, "Not channel operator");
		return ;
	}
	int paramIndex = 2;
	bool isAdding = false;

    for(size_t i = 0; i < modeString.length(); i++)
    {
        char mode = modeString[i];
        if (mode == '+') { isAdding = true; continue; }
        if (mode == '-') { isAdding = false; continue; }
        
        if (mode == 'i') 
			handleModeI(ch, isAdding);
        else if (mode == 't') 
			handleModeT(ch, isAdding);
        else if (mode == 'k')
		{
            if (paramIndex < (int)tokens.size())
                handleModeK(ch, isAdding, tokens[paramIndex++]);
        }
        else if (mode == 'o')
		{
            if (paramIndex < (int)tokens.size())
                handleModeO(ch, isAdding, ctx.services.users().findByNick(tokens[paramIndex++]));
        }
        else if (mode == 'l')
		{
            if (paramIndex < (int)tokens.size())
                handleModeL(ch, isAdding, std::atoi(tokens[paramIndex++].c_str()));
        }
        else if (mode == 'b')
        {
            if (paramIndex < (int)tokens.size())
                handleModeB(ch, isAdding, ctx.services.users().findByNick(tokens[paramIndex++]));
        }
    }
	std::string modeMessage = std::string(":") + ctx.sender->getNickname() + " MODE " + channelName + " " + modeString;

    for (int i = 2; i < (int)tokens.size(); i++)
    {
        modeMessage += " " + tokens[i];
    }
    const std::map<int, User*>& users = ch->getUsers();
    for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        ctx.services.sendToUser(it->second, modeMessage);
    }
}
