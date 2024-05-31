/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 17:10:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/EventHandler.hpp"
#include "irc/Server.hpp"
#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/Messages.hpp"
#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"

#include <algorithm>

using std::string;
using std::map;
using std::vector;

static void		checkConnection(const Client *client);
static void		checkAuthentication(const Client *client);
static string	get_next_token(string::iterator &it, const string::const_iterator &end, const char delim = ' ');

EventHandler::EventHandler(Logger &logger, Server &server) :
	_server(&server),
	_client(NULL),
	_commands(initCommands()),
	_handlers(initHandlers()),
	_logger(logger) {}

EventHandler::EventHandler(const EventHandler &copy) :
	_server(copy._server),
	_client(copy._client),
	_commands(copy._commands),
	_handlers(copy._handlers),
	_logger(copy._logger) {}
	
EventHandler::~EventHandler(void) {}

const map<string, e_commands>	&EventHandler::getCommands(void) const
{
	return _commands;
}

const Client	&EventHandler::getClient(void) const
{
	return *_client;
}

void	EventHandler::setClient(Client &client)
{
	_client = &client;
}

void	EventHandler::processInput(string &raw_input)
{
	vector<string> cmds = ::split(raw_input, "\r\n");

	for(uint8_t i = 0; i < cmds.size(); i++)
	{
		if (cmds[i].empty())
			continue ;
		try
		{
			const struct s_message input = parseInput(cmds[i]);
			(this->*(_handlers.at(static_cast<e_commands>(input.value))))(input.params);
		}
		catch (const ProtocolErrorException &e)
		{
			if (e.getContent().value == ERR_UNKNOWNCOMMAND)
			{
				_client->receiveMessage(e.getContent());
				_logger.logError(&e);
			}
			else
				throw ;
		}
	}
}

const struct s_message	EventHandler::buildMessage(const string &prefix, const int value, ...)
{
	va_list	args;

	va_start(args, value);
	const struct s_message content = buildMessage(prefix, value, args);
	va_end(args);
	return content;
}

const struct s_message EventHandler::buildMessage(const string &prefix, const int value, va_list args)
{
	struct s_message	content;
	const char			*param;

	content.prefix = prefix;
	content.value = value;
	while ((param = va_arg(args, const char *)) != NULL)
        content.params.push_back(param);
	return content;
}

void	EventHandler::sendBufferedMessage(const Client &receiver, const struct s_message &message)
{
	string		first_part, second_part, to_send;
	uint16_t	block_size, send_length;
	const int	receiver_socket = receiver.getSocket();

	unwrapMessage(message, &first_part, &second_part);
	block_size = MAX_MSG_LENGTH - first_part.size() - 2; //2 per \r\n
	while (!second_part.empty())
	{
		send_length = std::min(static_cast<size_t>(block_size),  second_part.size());
		to_send = first_part + second_part.substr(0, send_length) + "\r\n";
		//std::cout << "Sending: " << to_send << std::endl;
		second_part = second_part.substr(send_length); // Update second_part correctly
		send_p(receiver_socket, to_send.c_str(), to_send.length(), 0);
	}
}

void	EventHandler::unwrapMessage(const struct s_message &msg, string *first_part, string *second_part)
{
	const bool is_reply = msg.value >= RPL_WELCOME && msg.value <= ERR_CHANOPRIVSNEEDED;

	if (!msg.prefix.empty())
		*first_part += ":" + msg.prefix + " ";
	if (is_reply)
	{
		ostringstream oss;
		oss << std::setw(3) << std::setfill('0') << msg.value;	
		*first_part += oss.str();
	}
	else
		*first_part += _command_strings.at(msg.value);
	for (vector<string>::const_iterator it = msg.params.begin(); it != msg.params.end() - 1; it++)
		*second_part += " " + *it;
	if (!msg.params.empty())
		*second_part += " :" + msg.params.back();
}

const std::map<std::string, e_commands>	EventHandler::initCommands(void)
{
	static map<string, e_commands>	commands;

	if (!commands.empty()) //se il map e' gia' stato inizializzato
		return commands;

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

	return commands;
}

const map<e_commands, EventHandler::CommandHandler>	EventHandler::initHandlers(void)
{
	map<e_commands, CommandHandler>	handlers;

	handlers[PASS] = &EventHandler::handlePass;
	handlers[NICK] = &EventHandler::handleNick;
	handlers[USER] = &EventHandler::handleUser;
	handlers[JOIN] = &EventHandler::handleJoin;
	handlers[PART] = &EventHandler::handlePart;
	handlers[PRIVMSG] = &EventHandler::handlePrivmsg;
	handlers[QUIT] = &EventHandler::handleQuit;

	handlers[KICK] = &EventHandler::handleKick;
	handlers[INVITE] = &EventHandler::handleInvite;
	handlers[TOPIC] = &EventHandler::handleTopic;
	handlers[MODE] = &EventHandler::handleMode;
	
	return handlers;
}

std::map<uint16_t, std::string> EventHandler::initCommandStrings(void)
{
	static std::map<uint16_t, std::string>	command_strings;

	if (command_strings.empty())
	{
		command_strings[PASS] = "PASS";
		command_strings[NICK] = "NICK";
		command_strings[USER] = "USER";
		command_strings[JOIN] = "JOIN";
		command_strings[PART] = "PART";
		command_strings[PRIVMSG] = "PRIVMSG";
		command_strings[QUIT] = "QUIT";

		command_strings[KICK] = "KICK";
		command_strings[INVITE] = "INVITE";
		command_strings[TOPIC] = "TOPIC";
		command_strings[MODE] = "MODE";
	}
	return command_strings;
}

const struct s_message EventHandler::parseInput(string &raw_input) const
{
	struct s_message	input;
	string				command;
	string				param;
	string::iterator	it;

	if (*raw_input.rbegin() == '\n') // Remove trailing '\n
		raw_input.resize(raw_input.size() - 1);
	it = raw_input.begin();
	if (*it == ':')
		input.prefix = get_next_token(++it, raw_input.end(), ' '); // Extract the prefix
	command = get_next_token(it, raw_input.end(), ' '); // Extract the command
	const map<string, e_commands>::const_iterator it_command = _commands.find(command);
	if (it_command == _commands.end())
		throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command.c_str(), default_replies.at(ERR_UNKNOWNCOMMAND), NULL);
	input.value = it_command->second; // Associate the command with the enum
	while (it != raw_input.end())
	{
		if (*it == ':')
			param = get_next_token(++it, raw_input.end(), '\0'); // Extract the parameter
		else
			param = get_next_token(it, raw_input.end(), ' '); // Extract the parameter
		input.params.push_back(param); // Add the parameter to the vector
	}
	return input;	
}

void EventHandler::handlePass(const vector<string> &args)
{
	if (_client->getIsConnected())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED, default_replies.at(ERR_ALREADYREGISTRED), NULL);

	if (args.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PASS", "usage: PASS <password>", NULL);

	if (hash(args[0]) != _server->getPwdHash())
		throw ProtocolErrorException(ERR_PASSWDMISMATCH, default_replies.at(ERR_PASSWDMISMATCH), NULL);
	_client->setIsConnected(true);
}

void EventHandler::handleNick(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getNickname().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED, default_replies.at(ERR_ALREADYREGISTRED), NULL);
	if (args.size() < 1)
		throw ProtocolErrorException(ERR_NONICKNAMEGIVEN, default_replies.at(ERR_NONICKNAMEGIVEN), NULL);
	checkNicknameValidity(args[0]);
	_client->setNickname(args[0]);
	if (!_client->getUsername().empty())
		_client->setAuthenticated(true);
}

void EventHandler::handleUser(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getUsername().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED, default_replies.at(ERR_ALREADYREGISTRED), NULL);
	if (args.size() < 4)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "USER", "usage: USER <username> <hostname> <servername> <realname>", NULL);
	_client->setUsername(args[0]);
	(void)args[1]; //args[1] = hostname
	(void)args[2]; //args[2] = servername
	_client->setRealname(args[3]);
	if (!_client->getNickname().empty())
		_client->setAuthenticated(true);
}

//JOIN <channel1,channel2,channel3> <key1,key2,key3>
void EventHandler::handleJoin(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);
	if (args.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "JOIN", "usage: JOIN <channel>{,<channel>} [<key>{,<key>}]", NULL);

	const vector<string> 			channels_to_join = ::split(args[0], ",");
	const map<string, Channel *>	&channels = _server->getChannels();
	vector<string> 					keys;
	
	if (args.size() > 1)
		keys = ::split(args[1], ",");

	for (size_t i = 0; i < channels_to_join.size(); i++)
	{
		if (channels.find(channels_to_join[i]) == channels.end())
		{
			Channel	*new_channel;

			if (i < keys.size())
				new_channel = new Channel(_logger, channels_to_join[i],  *_client, keys[i]);
			else
				new_channel = new Channel(_logger, channels_to_join[i], *_client);
			_server->addChannel(*new_channel);
		}
		else
		{
			Channel &channel = _server->getChannel(channels_to_join[i]);
			if (i < keys.size())
				_client->joinChannel(channel, keys[i]);
			else
				_client->joinChannel(channel);
		}
	};
}

void EventHandler::handlePart(const vector<string> &args)
{
	const uint16_t n_args = args.size();
	
	if (n_args < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PART", "usage: PART <channel>{,<channel>} [<reason>]", NULL);

	const string reason = (n_args > 1) ? args[1] : "";
	const vector<string> channels = split(args[0], ",");

	for (vector<string>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel &channel = _server->getChannel(*it);
		_client->leaveChannel(channel, reason);
	}
}

void EventHandler::handlePrivmsg(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t n_args = args.size();
	if (n_args < 1)
		throw ProtocolErrorException(ERR_NORECIPIENT, "PRIVMSG", "no recipient given (PRIVMSG)", NULL);
	if (n_args < 2)
		throw ProtocolErrorException(ERR_NOTEXTTOSEND, default_replies.at(ERR_NOTEXTTOSEND), NULL);

	const struct s_message msg = EventHandler::buildMessage(_client->getNickname(), PRIVMSG, args[0].c_str(), args[1].c_str(), NULL);
	if (is_channel_prefix(args[0][0])) //se il primo carattere e' #, &, + o !
	{
		//channel msg PRIVMSG <channel> :<message>
		Channel &channel = _server->getChannel(args[0]);
		_client->sendMessage(channel, msg);
	}
	else
	{
		//private msg PRIVMSG <nickname> :<message>
		Client &receiver = _server->getClient(args[0]);
		_client->sendMessage(receiver, msg);
	}
}

void EventHandler::handleQuit(const vector<string> &args)
{
	const string					&reason = args.size() > 0 ? args[0] : "Client quit";
	const string					&quitting_nickname = _client->getNickname();
	const struct s_message			quit = EventHandler::buildMessage(quitting_nickname, QUIT, reason.c_str(), NULL);
	const map<string, Channel *>	&channels = _client->getChannels();
	
	for (map<string, Channel *>::const_iterator it_channel = channels.begin(); it_channel != channels.end(); it_channel++)
	{
		const Channel				*channel = it_channel->second;
		const map<string, Client *>	&members = channel->getMembers();

		for (map<string, Client *>::const_iterator it_member = members.begin(); it_member != members.end(); it_member++)
		{
			const Client	*member = it_member->second;

			if (member->getNickname() != quitting_nickname)
				member->receiveMessage(quit);
		}
	}
	_server->removeClient(*_client);
	_logger.logEvent("Client " + quitting_nickname + " has quit");
}

void EventHandler::handleKick(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t	n_args = args.size();
	if (n_args < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "KICK", "usage: KICK <channel> <nickname> [<reason>]", NULL);

	Channel	&channel = _server->getChannel(args[0]);
	Client	&target = _server->getClient(args[1]);

	args.size() > 2 ? _client->kick(target, channel, args[2]) : _client->kick(target, channel);
}

void EventHandler::handleInvite(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "INVITE", "usage: INVITE <nickname> <channel>", NULL);

	Client	&target = _server->getClient(args[0]);
	Channel	&channel = _server->getChannel(args[1]);

	_client->invite(target, channel);
}

void EventHandler::handleTopic(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const size_t	n_args = args.size();

	if (n_args < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "TOPIC", "usage: TOPIC <channel> [<topic>]", NULL);

	Channel &channel = _client->getChannel(args[0]);
	if (n_args == 1)
	{
		const string		&topic = channel.getTopic();
		const string		&channel_name = channel.getName();
		const string		&issuer_nickname = _client->getNickname();
		struct s_message	topic_reply;

		if (topic.empty())
			topic_reply = EventHandler::buildMessage(SERVER_NAME, RPL_NOTOPIC, issuer_nickname.c_str(), channel_name.c_str(), default_replies.at(RPL_NOTOPIC), NULL);
		else
			topic_reply = EventHandler::buildMessage(SERVER_NAME, RPL_TOPIC, issuer_nickname.c_str(), channel_name.c_str(), topic.c_str(), NULL);
		_client->receiveMessage(topic_reply);
	}
	else
		_client->topicSet(channel, args[1]);
}

void EventHandler::handleMode(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t	n_args = args.size();

	if (n_args < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}", NULL);

	Channel	&channel = _server->getChannel(args[0]);

	if (n_args == 1)
	{
		const map<char, bool>	&modes = channel.getModes();
		string					modes_str;

		for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second)
				modes_str += it->first;
		}
		if (!modes_str.empty())
			modes_str.insert(0, "+");
		const struct s_message	reply = EventHandler::buildMessage(SERVER_NAME, RPL_CHANNELMODEIS, _client->getNickname().c_str(), channel.getName().c_str(), modes_str.c_str(), NULL);
		_client->receiveMessage(reply);
		return;
	}

	if (args[1][0] != '+' && args[1][0] != '-')
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}", NULL);

	char				mode;
	bool				status;
	uint16_t			j = 2;
	map<char, bool>		new_modes;
	vector<string>		params;

	params.reserve(n_args - 2);
	for (uint32_t i = 0; i < args[1].size(); i++)
	{
		if (args[1][i] == '+' || args[1][i] == '-')
		{
			status = (args[1][i] == '+');
			continue ;
		}
		mode = args[1][i];
		if (string(SUPPORTED_CHANNEL_MODES).find(mode) == string::npos)
			throw ProtocolErrorException(ERR_UNKNOWNMODE, string(1, mode).c_str(), NULL);
		if (channel_mode_requires_param(mode, status))
		{
			if (j >= n_args)
				throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <channel> <mode> [<param>]", NULL);
			params.push_back(args[j++]);
		}
		new_modes[mode] = status;
	}
	_client->modesChange(channel, new_modes, params);
}

void	EventHandler::checkNicknameValidity(const string &nickname) const
{
	if (nickname.empty())
		throw ProtocolErrorException(ERR_NONICKNAMEGIVEN, default_replies.at(ERR_NONICKNAMEGIVEN), NULL);
	if (!is_valid_nickname(nickname))
	{
		const string nickname_rules_msg = "allowed characters: A-Z, a-z, 0-9, -, [, ], \\, `, ^, {, }\nmax nickname lenght: " + to_string(static_cast<int>(MAX_NICKNAME_LEN));
		throw ProtocolErrorException(ERR_ERRONEOUSNICKNAME, nickname.c_str(), nickname_rules_msg.c_str(), NULL);
	}
	if (_server->isClientConnected(nickname))
		throw ProtocolErrorException(ERR_NICKNAMEINUSE, nickname.c_str(), default_replies.at(ERR_NICKNAMEINUSE), NULL);
}

const std::map<uint16_t, std::string> EventHandler::_command_strings = EventHandler::initCommandStrings();

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not connected, use PASS <password> first", NULL);
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not registered, use NICK <nickname> and USER <username> first", NULL);
}

string	get_next_token(string::iterator &it, const string::const_iterator &end, const char delim)
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