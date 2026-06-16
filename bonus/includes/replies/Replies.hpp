/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:18 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:11:21 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define REPLIES_HPP

#include <string>

// IRC numerics and common reply templates.
// These macros build std::string directly.

#define IRC_CRLF "\r\n"
#define IRC_UMODES ""
#define IRC_CMODES "itkolmvb"
#define IRC_VERSION "1.0"

// Success / welcome replies.
#define RPL_WELCOME(client, nick, user, host) (std::string("001 ") + (client) + " :Welcome to the Internet Relay Network " + (nick) + "!" + (user) + "@" + (host) + IRC_CRLF)
#define RPL_YOURHOST(client, server) (std::string("002 ") + (client) + " :Your host is " + (server) + ", running version " + IRC_VERSION + IRC_CRLF)
#define RPL_CREATED(client, created) (std::string("003 ") + (client) + " :This server was created " + (created) + IRC_CRLF)
#define RPL_MYINFO(client, server) (std::string("004 ") + (client) + " " + (server) + " " + IRC_VERSION + " " + IRC_UMODES + " " + IRC_CMODES + IRC_CRLF)
#define RPL_ISUPPORT(client, tokens) (std::string("005 ") + (client) + " " + (tokens) + " :are supported by this server" + IRC_CRLF)

// Common channel / listing replies.
#define RPL_LISTSTART(client) (std::string("321 ") + (client) + " Channel :Users Name" + IRC_CRLF)
#define RPL_LIST(client, channel, visible, topic) (std::string("322 ") + (client) + " " + (channel) + " " + (visible) + " :" + (topic) + IRC_CRLF)
#define RPL_CHANNELMODEIS(client, channel, modes) (std::string("324 ") + (client) + " " + (channel) + " " + (modes) + IRC_CRLF)
#define RPL_NOTOPIC(client, channel) (std::string("331 ") + (client) + " " + (channel) + " :No topic is set" + IRC_CRLF)
#define RPL_INVITING(client, nick, channel) (std::string("341 ") + (client) + " " + (nick) + " " + (channel) + IRC_CRLF)
#define RPL_TOPIC(client, channel, topic) (std::string("332 ") + (client) + " " + (channel) + " :" + (topic) + IRC_CRLF)
#define RPL_TOPICWHOTIME(client, channel, nick, time) (std::string("333 ") + (client) + " " + (channel) + " " + (nick) + " " + (time) + IRC_CRLF)
#define RPL_NAMREPLY(client, symbol, channel, names) (std::string("353 ") + (client) + " " + (symbol) + " " + (channel) + " :" + (names) + IRC_CRLF)
#define RPL_ENDOFNAMES(client, channel) (std::string("366 ") + (client) + " " + (channel) + " :End of /NAMES list" + IRC_CRLF)
#define RPL_ENDOFLIST(client) (std::string("323 ") + (client) + " :End of /LIST" + IRC_CRLF)

// Common action replies.
#define RPL_JOIN(client, channel) (std::string(":") + (client) + " JOIN :" + (channel) + IRC_CRLF)
#define RPL_PART(client, channel, reason) (std::string(":") + (client) + " PART " + (channel) + " :" + (reason) + IRC_CRLF)
#define RPL_QUIT(client, reason) (std::string(":") + (client) + " QUIT :" + (reason) + IRC_CRLF)
#define RPL_PONG(server) (std::string("PONG ") + (server) + IRC_CRLF)
#define RPL_PRIVMSG(target, msg) (std::string("PRIVMSG ") + (target) + " :" + (msg) + IRC_CRLF)
#define RPL_KICK(client, channel, target, reason) (std::string(":") + (client) + " KICK " + (channel) + " " + (target) + " :" + (reason) + IRC_CRLF)

// Error replies.
#define ERR_NOSUCHNICK(client, target) (std::string("401 ") + (client) + " " + (target) + " :No such nick/channel" + IRC_CRLF)
#define ERR_NOSUCHSERVER(client, server) (std::string("402 ") + (client) + " " + (server) + " :No such server" + IRC_CRLF)
#define ERR_NOSUCHCHANNEL(client, channel) (std::string("403 ") + (client) + " " + (channel) + " :No such channel" + IRC_CRLF)
#define ERR_CANNOTSENDTOCHAN(client, channel) (std::string("404 ") + (client) + " " + (channel) + " :Cannot send to channel" + IRC_CRLF)
#define ERR_TOOMANYCHANNELS(client, channel) (std::string("405 ") + (client) + " " + (channel) + " :You have joined too many channels" + IRC_CRLF)
#define ERR_WASNOSUCHNICK(client) (std::string("406 ") + (client) + " :There was no such nickname" + IRC_CRLF)
#define ERR_NORECIPIENT(client, command) (std::string("411 ") + (client) + " " + (command) + " :No recipient given" + IRC_CRLF)
#define ERR_NOTEXTTOSEND(client, command) (std::string("412 ") + (client) + " " + (command) + " :No text to send" + IRC_CRLF)
#define ERR_UNKNOWNCOMMAND(client, command) (std::string("421 ") + (client) + " " + (command) + " :Unknown command" + IRC_CRLF)
#define ERR_NOMOTD(client) (std::string("422 ") + (client) + " :MOTD File is missing" + IRC_CRLF)
#define ERR_NONICKNAMEGIVEN(client) (std::string("431 ") + (client) + " :No nickname given" + IRC_CRLF)
#define ERR_ERRONEUSNICKNAME(client, nick) (std::string("432 ") + (client) + " " + (nick) + " :Erroneous nickname" + IRC_CRLF)
#define ERR_NICKNAMEINUSE(client, nick) (std::string("433 ") + (client) + " " + (nick) + " :Nickname is already in use" + IRC_CRLF)
#define ERR_USERNOTINCHANNEL(client, nick, channel) (std::string("441 ") + (client) + " " + (nick) + " " + (channel) + " :They aren't on that channel" + IRC_CRLF)
#define ERR_NOTONCHANNEL(client, channel) (std::string("442 ") + (client) + " " + (channel) + " :You're not on that channel" + IRC_CRLF)
#define ERR_USERONCHANNEL(client, nick, channel) (std::string("443 ") + (client) + " " + (nick) + " " + (channel) + " :is already on channel" + IRC_CRLF)
#define ERR_NOTREGISTERED(client) (std::string("451 ") + (client) + " :You have not registered" + IRC_CRLF)
#define ERR_NEEDMOREPARAMS(client, command) (std::string("461 ") + (client) + " " + (command) + " :Not enough parameters" + IRC_CRLF)
#define ERR_ALREADYREGISTERED(client) (std::string("462 ") + (client) + " :You may not reregister" + IRC_CRLF)
#define ERR_PASSWDMISMATCH(client) (std::string("464 ") + (client) + " :Password incorrect" + IRC_CRLF)
#define ERR_CHANNELISFULL(client, channel) (std::string("471 ") + (client) + " " + (channel) + " :Cannot join channel (+l)" + IRC_CRLF)
#define ERR_INVALIDMODEPARAM(client, channel) (std::string("696 ") + (client) + " " + (channel) + " :Invalid limit" + IRC_CRLF)
#define ERR_UNKNOWNMODE(client, mode) (std::string("472 ") + (client) + " " + (mode) + " :is unknown mode char to me" + IRC_CRLF)
#define ERR_INVITEONLYCHAN(client, channel) (std::string("473 ") + (client) + " " + (channel) + " :Cannot join channel (+i)" + IRC_CRLF)
#define ERR_BANNEDFROMCHAN(client, channel) (std::string("474 ") + (client) + " " + (channel) + " :Cannot join channel (+b)" + IRC_CRLF)
#define ERR_BADCHANNELKEY(client, channel) (std::string("475 ") + (client) + " " + (channel) + " :Cannot join channel (+k)" + IRC_CRLF)
#define ERR_BADCHANMASK(client, channel) (std::string("476 ") + (client) + " " + (channel) + " :Bad Channel Mask" + IRC_CRLF)
#define ERR_CHANOPRIVSNEEDED(client, channel) (std::string("482 ") + (client) + " " + (channel) + " :You're not channel operator" + IRC_CRLF)

#endif
