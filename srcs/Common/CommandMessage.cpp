/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:04:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 17:30:02 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMessage.hpp"
#include "common_exceptions.hpp"
#include "ReplyMessage.hpp"
#include "common_utils.hpp"

#include <cstdarg>

using std::map;
using std::string;

CommandMessage::CommandMessage(void)
{
}

CommandMessage::CommandMessage(const std::string &raw_input)
{
	parse(raw_input);
}

CommandMessage::CommandMessage(const string &prefix, int command, ...)
{
	va_list args;
	const char *param;

	if (command < PASS || command > MODE)
		throw InternalErrorException("CommandMessage::CommandMessage: invalid command");
	va_start(args, command);
	_prefix = prefix;
	_command = static_cast<e_commands>(command);
	while ((param = va_arg(args, const char *)) != NULL)
		_params.push_back(param);
	va_end(args);
}

CommandMessage::CommandMessage(const string &prefix, const enum e_commands command, va_list args)
{
	const char *param;

	_prefix = prefix;
	_command = command;
	while ((param = va_arg(args, const char *)) != NULL)
		_params.push_back(param);
}

CommandMessage::CommandMessage(const CommandMessage &copy) : AMessage(copy),
															 _command(copy._command) {}

CommandMessage::~CommandMessage(void) {}

CommandMessage &CommandMessage::operator=(const CommandMessage &copy)
{
	if (this != &copy)
	{
		AMessage::operator=(copy);
		_command = copy._command;
	}
	return (*this);
}

void CommandMessage::parse(string raw_input)
{
	string command;
	string param;
	string::iterator it;

	if (*raw_input.rbegin() == '\n')
		raw_input.resize(raw_input.size() - 1);
	it = raw_input.begin();
	if (*it == ':')
		_prefix = get_next_token(++it, raw_input.end(), ' ');
	command = get_next_token(it, raw_input.end(), ' ');
	const map<string, e_commands>::const_iterator it_command = g_commands.find(command);
	if (it_command == g_commands.end())
		_command = UNKNOWN;
	else
		_command = it_command->second;
	while (it != raw_input.end())
	{
		if (*it == ':')
			param = get_next_token(++it, raw_input.end(), '\0');
		else
			param = get_next_token(it, raw_input.end(), ' ');
		_params.push_back(param);
	}
}

void CommandMessage::unwrapMessage(string &first_part, string &second_part) const
{
	if (!_prefix.empty())
		first_part += ":" + _prefix + " ";
	for (map<string, enum e_commands>::const_iterator it = g_commands.begin(); it != g_commands.end(); it++)
	{
		if (it->second == _command)
		{
			first_part += it->first;
			break;
		}
	}
	for (vector<string>::const_iterator it = _params.begin(); it != _params.end() - 1; it++)
		second_part += " " + *it;
	if (!_params.empty())
		second_part += " :" + _params.back();
}

const std::map<std::string, enum e_commands> g_commands = create_commands_map();

const std::map<std::string, enum e_commands> create_commands_map(void)
{
	std::map<std::string, enum e_commands> commands;

	commands["PASS"] = PASS;
	commands["NICK"] = NICK;
	commands["USER"] = USER;
	commands["JOIN"] = JOIN;
	commands["PART"] = PART;
	commands["PRIVMSG"] = PRIVMSG;
	commands["QUIT"] = QUIT;

	commands["KICK"] = KICK;
	commands["INVITE"] = INVITE;
	commands["TOPIC"] = TOPIC;
	commands["MODE"] = MODE;
	return (commands);
}