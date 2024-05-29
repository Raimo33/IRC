/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/29 15:13:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/EventHandler.hpp"
#include "irc/Server.hpp"
#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"

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

const map<string, e_cmd_type>	&EventHandler::getCommands(void) const
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

void	EventHandler::processInput(string raw_input)
{
	raw_input = raw_input.substr(0, MAX_MSG_LENGTH); // Limit the length of the input to prevent buffer overflow
	if (has_crlf(raw_input))
		raw_input.resize(raw_input.size() - 2); // Remove \r\n

	vector<string> cmds = ::split(raw_input, '\n');

	for(uint8_t i = 0; i < cmds.size(); i++)
	{
		if (cmds[i].empty())
			continue ;
		s_commandMessage input = parseInput(cmds[i]);
		(this->*(_handlers[input.cmd]))(input.params);
	}
}

const struct s_replyMessage	EventHandler::buildReplyMessage(const enum e_replyCodes code, const vector<string> &params, const string &custom_msg)
{
	struct s_replyMessage	content;

	content.prefix = string(SERVER_NAME);
	content.code = code;
	content.params = params;
	if (custom_msg.empty())
	{
		map<enum e_replyCodes, string>::const_iterator it = reply_codes.find(code);
		if (it == reply_codes.end())
			throw InternalErrorException("EventHandler::buildReplyMessage: reply code does not match any reply string");
		content.text = it->second;
	}
	else
		content.text = custom_msg;
	return content;
}

const struct s_replyMessage	EventHandler::buildReplyMessage(const enum e_replyCodes code, const string &param, const string &custom_msg)
{
	const vector<string>	params(1, param);

	return buildReplyMessage(code, params, custom_msg);
}

const struct s_commandMessage	EventHandler::buildCommandMessage(const string &prefix, const e_cmd_type cmd, const vector<string> &params, const string &custom_msg)
{
	struct s_commandMessage	content;

	content.prefix = prefix;
	content.cmd = cmd;
	content.params = params;
	content.text = custom_msg;
	return content;
}

const struct s_commandMessage	EventHandler::buildCommandMessage(const string &prefix, const e_cmd_type cmd, const string param, const string &custom_msg)
{
	const vector<string>	params(1, param);
	
	return buildCommandMessage(prefix, cmd, params, custom_msg);
}

void	EventHandler::sendBufferedContent(const Client &receiver, const struct s_messageBase *message) //TODO refactor con iterators
{
	string							first_part;
	string							second_part;
	uint16_t						block_size;
	uint16_t						send_length;
	string							to_send;
	const struct s_replyMessage		*reply = dynamic_cast<const s_replyMessage *>(message);
	const struct s_commandMessage	*command = dynamic_cast<const s_commandMessage *>(message);
	

	if (!reply && !command)
		throw InternalErrorException("EventHandler::sendBufferedContent: Invalid message type");
	if (command && (command->cmd < 0 || command->cmd >= N_COMMANDS))
		throw InternalErrorException("EventHandler::sendBufferedContent: Invalid command type");
	if (reply && reply_codes.find(reply->code) == reply_codes.end())
		throw InternalErrorException("EventHandler::sendBufferedContent: Invalid reply code");

	const int receiver_socket = receiver.getSocket();

	if (reply)
		getRawReplyMessage(receiver, reply, &first_part, &second_part);
	else
		getRawCommandMessage(command, &first_part, &second_part);
	block_size = MAX_MSG_LENGTH - first_part.size() - 2; //2 per \r\n
	do {
		send_length = min(static_cast<size_t>(block_size), second_part.size());
		to_send = first_part + second_part.substr(0, send_length) + "\r\n";
		second_part = second_part.substr(send_length); // Update second_part correctly
		send_p(receiver_socket, to_send.c_str(), to_send.length(), 0);
	} while (!second_part.empty());
}

void	EventHandler::getRawReplyMessage(const Client &receiver, const struct s_replyMessage *reply, string *first_part, string *second_part)
{
	if (!reply->prefix.empty())
		*first_part += ":" + reply->prefix + " ";
	*first_part += ::to_string(reply->code);
	*first_part += " " + receiver.getNickname();
	if (!reply->params.empty() && !reply->params[0].empty())
		*first_part += " " + join(reply->params, " ");
	if (reply->text.empty())
		*second_part += " :" + reply_codes.at(reply->code);
	else
		*second_part += " :" + reply->text;
}

void	EventHandler::getRawCommandMessage(const struct s_commandMessage *command, string *first_part, string *second_part)
{
	if (!command->prefix.empty())
		*first_part += ":" + command->prefix + " ";
	*first_part += _command_strings.at(command->cmd);
	if (!command->params.empty() && !command->params[0].empty())
		*first_part += " " + join(command->params, " ");
	*second_part += " :" + command->text;
}

const std::map<std::string, e_cmd_type>	EventHandler::initCommands(void)
{
	static map<string, e_cmd_type>	commands;

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

const vector<EventHandler::CommandHandler>	EventHandler::initHandlers(void)
{
	vector<CommandHandler>	handlers(N_COMMANDS);

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

struct s_commandMessage EventHandler::parseInput(string &raw_input) const
{
	struct s_commandMessage	input;
	string					command;
	string					param;
	string::iterator		it;

	it = raw_input.begin();
	if (*it == ':')
		input.prefix = get_next_token(++it, raw_input.end(), ' '); // Extract the prefix
	command = get_next_token(it, raw_input.end(), ' '); // Extract the command
	const map<string, e_cmd_type>::const_iterator it_command = _commands.find(command);
	if (it_command == _commands.end())
		throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command);
	input.cmd = it_command->second; // Associate the command with the enum
	while (it != raw_input.end())
	{
		if (*it == ':')
			param = get_next_token(++it, raw_input.end(), '\0');
		else
			param = get_next_token(it, raw_input.end(), ' '); // Extract the parameter
		input.params.push_back(param); // Add the parameter to the vector
	}
	return input;	
}

void EventHandler::handlePass(const vector<string> &args)
{
	if (_client->getIsConnected())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);

	if (args.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PASS", "usage: PASS <password>");

	if (hash(args[0]) != _server->getPwdHash())
		throw ProtocolErrorException(ERR_PASSWDMISMATCH);
	_client->setIsConnected(true);
}

void EventHandler::handleNick(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getNickname().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);
	if (args.size() < 1)
		throw ProtocolErrorException(ERR_NONICKNAMEGIVEN);
	checkNicknameValidity(args[0]);
	_client->setNickname(args[0]);
	if (!_client->getUsername().empty())
		_client->setAuthenticated(true);
}

void EventHandler::handleUser(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getUsername().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);
	if (args.size() < 4)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "USER", "usage: USER <username> <hostname> <servername> <realname>");
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
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "JOIN", "usage: JOIN <channel>{,<channel>} [<key>{,<key>}]");

	const vector<string> 			channels_to_join = split(args[0], ',');
	const map<string, Channel *>	&channels = _server->getChannels();
	vector<string> 					keys;
	
	if (args.size() > 1)
		keys = split(args[1], ',');

	for (size_t i = 0; i < channels_to_join.size(); i++)
	{
		if (channels.find(channels_to_join[i]) == channels.end())
		{
			Channel			*new_channel;

			if (i < keys.size())
				new_channel = new Channel(_logger, channels_to_join[i],  *_client, keys[i]);
			else
				new_channel = new Channel(_logger, channels_to_join[i], *_client);
			_server->addChannel(*new_channel);
		}
		else
		{
			Channel *channel = _server->getChannel(channels_to_join[i]);
			if (i < keys.size())
				_client->joinChannel(*channel, keys[i]);
			else
				_client->joinChannel(*channel);
		}
	};
}

void EventHandler::handlePart(const vector<string> &args)
{
	const uint16_t n_args = args.size();
	
	if (n_args < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PART", "usage: PART <channel>{,<channel>} [<reason>]");

	const string reason = (n_args > 1) ? args[1] : "";
	const vector<string> channels = split(args[0], ',');

	for (vector<string>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = _server->getChannel(*it);
		_client->leaveChannel(*channel, reason);
	}
}

void EventHandler::handlePrivmsg(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t n_args = args.size();
	if (n_args < 1)
		throw ProtocolErrorException(ERR_NORECIPIENT, "PRIVMSG", "no recipient given (PRIVMSG)");
	if (n_args < 2)
		throw ProtocolErrorException(ERR_NOTEXTTOSEND);

	const struct s_commandMessage msg = EventHandler::buildCommandMessage(_client->getNickname(), PRIVMSG, "", args[1]);
	if (is_channel_prefix(args[0][0])) //se il primo carattere e' #, &, + o !
	{
		//channel msg PRIVMSG <channel> :<message>
		Channel *channel = _server->getChannel(args[0]);
		_client->sendMessage(*channel, msg);
	}
	else
	{
		//private msg PRIVMSG <nickname> :<message>
		Client *receiver = _server->getClient(args[0]);
		_client->sendMessage(*receiver, msg);
	}
}

void EventHandler::handleQuit(const vector<string> &args)
{
	const string					&reason = args.size() > 0 ? args[0] : "Client quit";
	const string					&quitting_nickname = _client->getNickname();
	const struct s_commandMessage	quit = EventHandler::buildCommandMessage(quitting_nickname, QUIT, "", reason);
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
}

void EventHandler::handleKick(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t	n_args = args.size();
	if (n_args < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "KICK", "usage: KICK <channel> <nickname> [<reason>]");

	Channel	*channel = _server->getChannel(args[0]);
	Client	*target = _server->getClient(args[1]);

	args.size() > 2 ? _client->kick(*target, *channel, args[2]) : _client->kick(*target, *channel);
}

void EventHandler::handleInvite(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "INVITE", "usage: INVITE <nickname> <channel>");

	Client	*target = _server->getClient(args[0]);
	Channel	*channel = _server->getChannel(args[1]);

	_client->invite(*target, *channel);
}

void EventHandler::handleTopic(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const size_t	n_args = args.size();

	if (n_args < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "TOPIC", "usage: TOPIC <channel> [<topic>]");

	Channel &channel = _client->getChannel(args[0]);
	if (n_args == 1)
	{
		const string &topic = channel.getTopic();
		struct s_replyMessage topic_reply;

		if (topic.empty())
			topic_reply = EventHandler::buildReplyMessage(RPL_NOTOPIC, channel.getName());
		else
		{
			vector<string> params;
			params.push_back(_client->getNickname());
			params.push_back(channel.getName());
			topic_reply = EventHandler::buildReplyMessage(RPL_TOPIC, params, topic);
		}
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
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}");

	Channel			*channel = _server->getChannel(args[0]);

	if (n_args == 1)
	{
		const map<char, bool>	&modes = channel->getModes();
		string					modes_str("+");

		for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second)
				modes_str += it->first;
		}

		const struct s_replyMessage	reply = EventHandler::buildReplyMessage(RPL_CHANNELMODEIS, channel->getName(), modes_str);
		_client->receiveMessage(reply);
		return;
	}

	if (args[1][0] != '+' && args[1][0] != '-')
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}");

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
			throw ProtocolErrorException(ERR_UNKNOWNMODE, string(1, mode));
		if (channel_mode_requires_param(mode, status))
		{
			if (j >= n_args)
				throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <channel> <mode> [<param>]");
			params.push_back(args[j++]);
		}
		new_modes[mode] = status;
	}
	_client->modesChange(*channel, new_modes, params);
}

void	EventHandler::checkNicknameValidity(const string &nickname) const
{
	if (nickname.empty())
		throw ProtocolErrorException(ERR_NONICKNAMEGIVEN);
	if (!is_valid_nickname(nickname))
		throw ProtocolErrorException(ERR_ERRONEOUSNICKNAME, nickname, "allowed characters: A-Z, a-z, 0-9, -, [, ], \\, `, ^, {, }\nmax nickname lenght: " + to_string(static_cast<int>(MAX_NICKNAME_LEN)));
	if (_server->isClientConnected(nickname))
		throw ProtocolErrorException(ERR_NICKNAMEINUSE, nickname);
}

const std::map<uint16_t, std::string> EventHandler::_command_strings = EventHandler::initCommandStrings();

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not connected, use PASS <password> first");
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not registered, use NICK <nickname> and USER <username> first");
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
