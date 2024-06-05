/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:54:30 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 13:35:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReplyMessage.hpp"
#include "common_exceptions.hpp"
#include "common_utils.hpp"

#include <iomanip>
#include <sstream>

using std::istringstream;
using std::make_pair;
using std::map;
using std::ostringstream;
using std::string;
using std::vector;

ReplyMessage::ReplyMessage(void) {}

ReplyMessage::ReplyMessage(const string &raw_input)
{
	parse(raw_input);
}

ReplyMessage::ReplyMessage(const string &prefix, const int reply_code, ...)
{
	va_list		args;
	const char *param;

	if (reply_code < RPL_WELCOME || reply_code > ERR_CHANOPRIVSNEEDED)
		throw InternalErrorException("ReplyMessage::ReplyMessage: invalid reply code");
	_prefix = prefix;
	_reply_code = static_cast<enum e_replyCodes>(reply_code);
	va_start(args, reply_code);
	while ((param = va_arg(args, const char *)) != NULL)
		_params.push_back(param);
	va_end(args);
}

ReplyMessage::ReplyMessage(const string &prefix, const enum e_replyCodes reply_code, va_list args)
{
	const char *param;

	_prefix = prefix;
	_reply_code = reply_code;
	while ((param = va_arg(args, const char *)) != NULL)
		_params.push_back(param);
}

ReplyMessage::ReplyMessage(const string &prefix, const enum e_replyCodes reply_code, const vector<string> &params) :
	_reply_code(reply_code)
{
	_prefix = prefix;
	_params = params;
}

ReplyMessage::ReplyMessage(const ReplyMessage &copy) :
	AMessage(copy),
	_reply_code(copy._reply_code) {}

ReplyMessage::~ReplyMessage(void) {}

ReplyMessage &ReplyMessage::operator=(const ReplyMessage &copy)
{
	if (this != &copy)
	{
		AMessage::operator=(copy);
		_reply_code = copy._reply_code;
	}
	return (*this);
}

enum e_replyCodes ReplyMessage::getReplyCode(void) const { return (_reply_code); }
void			  ReplyMessage::setReplyCode(const enum e_replyCodes reply_code) { _reply_code = reply_code; }

void ReplyMessage::parse(string raw_input)
{
	string			 reply_code_str;
	string			 param;
	string::iterator it;
	istringstream	 iss;
	uint16_t		 reply_code;

	if (raw_input.empty())
		return;
	if (*raw_input.rbegin() == '\n')
		raw_input.resize(raw_input.size() - 1);
	it = raw_input.begin();
	if (*it == ':')
		_prefix = get_next_token(++it, raw_input.end(), ' ');
	reply_code_str = get_next_token(it, raw_input.end(), ' ');
	iss.str(reply_code_str);
	iss >> reply_code;
	if (reply_code < RPL_WELCOME || reply_code > ERR_CHANOPRIVSNEEDED)
		_reply_code = RPL_UNKNOWN;
	else
		_reply_code = static_cast<enum e_replyCodes>(reply_code);
	while (it != raw_input.end())
	{
		if (*it == ':')
			param = get_next_token(++it, raw_input.end(), '\0');
		else
			param = get_next_token(it, raw_input.end(), ' ');
		_params.push_back(param);
	}
}

void ReplyMessage::unwrapMessage(string &first_part, string &second_part) const
{
	if (!_prefix.empty())
		first_part += ":" + _prefix + " ";
	ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << _reply_code;
	first_part += oss.str();
	for (vector<string>::const_iterator it = _params.begin(); it != _params.end() - 1; it++)
		second_part += " " + *it;
	if (!_params.empty())
		second_part += " :" + _params.back();
}

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

const map<enum e_replyCodes, const char *> g_default_replies_map = create_default_replies_map();