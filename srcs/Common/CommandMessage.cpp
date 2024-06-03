/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:04:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 16:50:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMessage.hpp"
#include "common_exceptions.hpp"
#include "ReplyMessage.hpp"
#include "common_utils.hpp"

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

void CommandMessage::setCommand(const enum e_commands command)
{
	_command = command;
}

const enum e_commands &CommandMessage::getCommand(void) const
{
	return (_command);
}

enum e_commands CommandMessage::getCommandFromString(const string &command)
{
	const map<string, enum e_commands>::const_iterator it = g_str_cmd_map.find(command);

	if (it == g_str_cmd_map.end())
		return (CMD_UNKNOWN);
	return (it->second);
}

const string CommandMessage::getStringFromCommand(const enum e_commands command)
{
	const map<enum e_commands, string>::const_iterator it = g_cmd_str_map.find(command);

	if (it == g_cmd_str_map.end())
		return ("");
	return (it->second);
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
	const map<string, e_commands>::const_iterator it_command = g_str_cmd_map.find(command);
	if (it_command == g_str_cmd_map.end())
		_command = CMD_UNKNOWN;
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
	for (map<string, enum e_commands>::const_iterator it = g_str_cmd_map.begin(); it != g_str_cmd_map.end(); it++)
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

const map<string, enum e_commands> create_str_cmd_map(void)
{
	map<string, enum e_commands> str_cmd_map;

	str_cmd_map["PASS"] = PASS;
	str_cmd_map["NICK"] = NICK;
	str_cmd_map["USER"] = USER;
	str_cmd_map["JOIN"] = JOIN;
	str_cmd_map["PART"] = PART;
	str_cmd_map["PRIVMSG"] = PRIVMSG;
	str_cmd_map["QUIT"] = QUIT;
	str_cmd_map["SEND"] = SEND;
	str_cmd_map["KICK"] = KICK;
	str_cmd_map["INVITE"] = INVITE;
	str_cmd_map["TOPIC"] = TOPIC;
	str_cmd_map["MODE"] = MODE;
	return (str_cmd_map);
}

const map<enum e_commands, string> create_cmd_str_map(void)
{
	map<enum e_commands, string> cmd_str_map;

	cmd_str_map[PASS] = "PASS";
	cmd_str_map[NICK] = "NICK";
	cmd_str_map[USER] = "USER";
	cmd_str_map[JOIN] = "JOIN";
	cmd_str_map[PART] = "PART";
	cmd_str_map[PRIVMSG] = "PRIVMSG";
	cmd_str_map[QUIT] = "QUIT";
	cmd_str_map[SEND] = "SEND";
	cmd_str_map[KICK] = "KICK";
	cmd_str_map[INVITE] = "INVITE";
	cmd_str_map[TOPIC] = "TOPIC";
	cmd_str_map[MODE] = "MODE";
	return (cmd_str_map);
}

const map<string, e_commands> g_str_cmd_map = create_str_cmd_map();
const map<e_commands, string> g_cmd_str_map = create_cmd_str_map();