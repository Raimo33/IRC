/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:04:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 15:40:21 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMessage.hpp"
#include "Exceptions.hpp"
#include "ReplyMessage.hpp"

#include <cstdarg>

using std::string;
using std::map;

static string get_next_token(string::iterator &it, const string::const_iterator &end, const char delim);

CommandMessage::CommandMessage(void)
{
}

CommandMessage::CommandMessage(const std::string &raw_input)
{
	parse(raw_input);
}

CommandMessage::CommandMessage(const string &prefix, const enum e_commands command, ...)
{
	va_list		args;
	const char	*param;

	va_start(args, command);
	_prefix = prefix;
	_command = command;
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

CommandMessage::CommandMessage(const CommandMessage &copy) :
	AMessage(copy),
	_command(copy._command) {}

CommandMessage::~CommandMessage(void) {}

CommandMessage	&CommandMessage::operator=(const CommandMessage &copy)
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
		throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command.c_str(), g_default_replies.at(ERR_UNKNOWNCOMMAND), NULL);
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

static string get_next_token(string::iterator &it, const string::const_iterator &end, const char delim)
{
	const string::iterator start = it;
	string token;

	while (it != end && *it == delim) // Skip leading delimiters
		it++;
	while (it != end && *it != delim) // Extract the token
		it++;
	token = string(start, it);
	while (it != end && *it == delim) // Skip trailing delimiters
		it++;
	return token;
}

const std::map<std::string, enum e_commands> g_commands = create_commands_map();

const std::map<std::string, enum e_commands>	create_commands_map(void)
{
	std::map<std::string, enum e_commands>	commands;

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
}