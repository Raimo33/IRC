/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 02:55:58 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 19:54:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Messages.hpp"

#include <map>
#include <string>

using std::string;
using std::map;
using std::make_pair;

s_message::s_message(void) {}

s_message::s_message(const string &prefix, const int value, ...)
{
	va_list		args;
	const char	*param;

	va_start(args, value);
	this->prefix = prefix;
	this->value = value;
	while ((param = va_arg(args, const char *)) != NULL)
        params.push_back(param);
	va_end(args);
}

s_message::s_message(const string &prefix, const int value, va_list args)
{
	const char	*param;

	this->prefix = prefix;
	this->value = value;
	while ((param = va_arg(args, const char *)) != NULL)
        params.push_back(param);
}

s_message &s_message::operator=(const s_message &other)
{
	if (this == &other)
		return *this;
	prefix = other.prefix;
	value = other.value;
	params = other.params;
	return *this;
}

const map<enum e_replyCodes, const char *> create_default_replies(void)
{
	map<enum e_replyCodes, const char *> default_replies;
	default_replies.insert(make_pair(RPL_WELCOME, "Welcome to the Internet Relay Network"));
	default_replies.insert(make_pair(RPL_YOURHOST, "Your host is running version"));
	default_replies.insert(make_pair(RPL_CHANNELMODEIS, "Channel mode is"));
	default_replies.insert(make_pair(RPL_NOTOPIC, "No topic is set"));
	default_replies.insert(make_pair(RPL_TOPIC, "Topic"));
	default_replies.insert(make_pair(RPL_INVITING, "Inviting"));
	default_replies.insert(make_pair(RPL_NAMREPLY, "Name reply"));
	default_replies.insert(make_pair(RPL_ENDOFNAMES, "End of /NAMES list"));
	default_replies.insert(make_pair(RPL_YOUREOPER, "You are now an IRC operator"));
	default_replies.insert(make_pair(RPL_NOTOPERANYMORE, "You are no longer an IRC operator"));
	default_replies.insert(make_pair(ERR_NOSUCHNICK, "No such nick/channel"));
	default_replies.insert(make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	default_replies.insert(make_pair(ERR_TOOMANYCHANNELS, "You have joined too many channels"));
	default_replies.insert(make_pair(ERR_NORECIPIENT, "No recipient given"));
	default_replies.insert(make_pair(ERR_NOTEXTTOSEND, "No text to send"));
	default_replies.insert(make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
	default_replies.insert(make_pair(ERR_FILEERROR, "File error"));
	default_replies.insert(make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
	default_replies.insert(make_pair(ERR_ERRONEOUSNICKNAME, "Erroneous nickname"));
	default_replies.insert(make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
	default_replies.insert(make_pair(ERR_USERNOTINCHANNEL, "They aren't on that channel"));
	default_replies.insert(make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
	default_replies.insert(make_pair(ERR_USERONCHANNEL, "is already on channel"));
	default_replies.insert(make_pair(ERR_NOLOGIN, "User not logged in"));
	default_replies.insert(make_pair(ERR_NOTREGISTERED, "You have not registered"));
	default_replies.insert(make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	default_replies.insert(make_pair(ERR_ALREADYREGISTRED, "You may not reregister"));
	default_replies.insert(make_pair(ERR_PASSWDMISMATCH, "Password incorrect"));
	default_replies.insert(make_pair(ERR_CHANNELISFULL, "Cannot join channel (+l)"));
	default_replies.insert(make_pair(ERR_UNKNOWNMODE, "is unknown mode char to me"));
	default_replies.insert(make_pair(ERR_INVITEONLYCHAN, "Cannot join channel (+i)"));
	default_replies.insert(make_pair(ERR_BADCHANNELKEY, "Cannot join channel (+k)"));
	default_replies.insert(make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
	return default_replies;
}

const map<enum e_replyCodes, const char *> default_replies = create_default_replies();
