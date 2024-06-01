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

#include "Message.hpp"
#include "Exceptions.hpp"

//TODO rimettere parseMessage a parte, questo deve essere in comune tra BOT e SERVER

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
        _params.push_back(param);
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



