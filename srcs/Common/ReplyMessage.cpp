/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:54:30 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 15:39:19 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReplyMessage.hpp"

using std::map;
using std::make_pair;

ReplyMessage::ReplyMessage(void) {}

ReplyMessage::ReplyMessage(const std::string &raw_input)
{
	parse(raw_input);
}

ReplyMessage::ReplyMessage(const ReplyMessage &copy) :
	AMessage(copy),
	_reply_code(copy._reply_code) {}

ReplyMessage::~ReplyMessage(void) {}

ReplyMessage	&ReplyMessage::operator=(const ReplyMessage &copy)
{
	if (this != &copy)
	{
		AMessage::operator=(copy);
		_reply_code = copy._reply_code;
	}
	return (*this);
}

enum e_replyCodes	ReplyMessage::getReplyCode(void) const { return (_reply_code); }
void				ReplyMessage::setReplyCode(const enum e_replyCodes reply_code) { _reply_code = reply_code; }

void				ReplyMessage::parse(std::string raw_input)
{
	//TODO: implement
}

const map<enum e_replyCodes, const char *> g_default_replies = create_default_replies_map();

const map<enum e_replyCodes, const char *> create_default_replies_map(void)
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
