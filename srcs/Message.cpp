/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 02:55:58 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 19:54:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Message.hpp"
#include "irc/Exceptions.hpp"

#include <map>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::map;
using std::make_pair;

static string	get_next_token(string::iterator &it, const string::const_iterator &end, const char delim);

Message::Message(void)
{
	initCommands();
}

Message::Message(const Message &copy) :
	_prefix(copy._prefix),
	_value(copy._value),
	_params(copy._params),
	_commands(copy._commands) {}

Message::Message(const string &raw_input)
{
	initCommands();
	parse(raw_input);
}

Message::Message(const string &prefix, const int value, ...)
{
	va_list		args;
	const char	*param;

	initCommands();
	va_start(args, value);
	_prefix = prefix;
	_value = value;
	while ((param = va_arg(args, const char *)) != NULL)
        _params.push_back(param);
	va_end(args);
}

Message::Message(const string &prefix, const int value, va_list args)
{
	const char	*param;

	initCommands();
	_prefix = prefix;
	_value = value;
	while ((param = va_arg(args, const char *)) != NULL)
        setParam(param);
}

Message::~Message(void) {}

Message &Message::operator=(const Message &other)
{
	if (this == &other)
		return *this;
	_prefix = other._prefix;
	_value = other._value;
	_params = other._params;
	_commands = other._commands;
	return *this;
}

const string	&Message::getPrefix(void) const
{
	return _prefix;
}

void	Message::setPrefix(const string &prefix)
{
	_prefix = prefix;
}

uint16_t	Message::getValue(void) const
{
	return _value;
}

void	Message::setValue(const uint16_t value)
{
	_value = value;
}

const vector<string>	&Message::getParams(void) const
{
	return _params;
}

void	Message::setParams(const vector<string> &params)
{
	_params = params;
}

void	Message::setParam(const string &param, int32_t index)
{
	if (_params.empty())
	{
		_params.push_back(param);
		return;
	}
	if (index < 0)
		index += _params.size();

	if (index < 0 || index > static_cast<int32_t>(_params.size()))
		throw InternalErrorException("Message::setParam: Index out of range");
	_params.insert(_params.begin() + index, param);
}

const map<string, e_commands>	&Message::getCommands(void) const
{
	return _commands;
}

void	Message::setCommands(const map<string, e_commands> &commands)
{
	_commands = commands;
}

void	Message::initCommands(void)
{
	if (!_commands.empty())
		return;

	_commands["PASS"] = PASS;
	_commands["NICK"] = NICK;
	_commands["USER"] = USER;
	_commands["JOIN"] = JOIN;
	_commands["PART"] = PART;
	_commands["PRIVMSG"] = PRIVMSG;
	_commands["QUIT"] = QUIT;

	_commands["KICK"] = KICK;
	_commands["INVITE"] = INVITE;
	_commands["TOPIC"] = TOPIC;
	_commands["MODE"] = MODE;
}

void Message::parse(string raw_input)
{
	string				command;
	string				param;
	string::iterator	it;

	if (*raw_input.rbegin() == '\n')
		raw_input.resize(raw_input.size() - 1);
	it = raw_input.begin();
	if (*it == ':')
		_prefix = get_next_token(++it, raw_input.end(), ' ');
	command = get_next_token(it, raw_input.end(), ' ');
	const map<string, e_commands>::const_iterator it_command = _commands.find(command);
	if (it_command == _commands.end())
		throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command.c_str(), default_replies.at(ERR_UNKNOWNCOMMAND), NULL);
	_value = it_command->second;
	while (it != raw_input.end())
	{
		if (*it == ':')
			param = get_next_token(++it, raw_input.end(), '\0');
		else
			param = get_next_token(it, raw_input.end(), ' ');
		_params.push_back(param);
	}
}

static string	get_next_token(string::iterator &it, const string::const_iterator &end, const char delim)
{
	const string::iterator	start = it;
	string					token;

	while (it != end && *it == delim) // Skip leading delimiters
		it++;
	while (it != end && *it != delim) // Extract the token
		it++;
	token = string(start, it);
	while (it != end && *it == delim) // Skip trailing delimiters
		it++;
	return token;
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

