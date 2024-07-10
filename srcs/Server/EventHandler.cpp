/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/07/10 16:53:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "CommandMessage.hpp"
#include "ReplyMessage.hpp"
#include "Server.hpp"
#include "server_constants.hpp"
#include "server_exceptions.hpp"
#include "server_utils.hpp"

#include <algorithm>

using std::map;
using std::string;
using std::vector;

static void checkConnection(const Client *client);
static void checkAuthentication(const Client *client);

EventHandler::EventHandler(Logger &logger, Server &server) :
	_server(&server),
	_client(NULL),
	_handlers(initHandlers()),
	_logger(logger) {}

EventHandler::EventHandler(const EventHandler &copy) :
	_server(copy._server),
	_client(copy._client),
	_handlers(copy._handlers),
	_logger(copy._logger) {}

EventHandler::~EventHandler(void) {}

const Client &EventHandler::getClient(void) const
{
	return *_client;
}

void EventHandler::setClient(Client &client)
{
	_client = &client;
}

void EventHandler::processInput(string raw_input)
{
	vector<string>	inputs = ::split(raw_input, "\r\n");
	uint16_t		n_inputs = inputs.size();

	for (uint16_t i = 0; i < n_inputs; i++)
	{
		if (inputs[i].empty())
			continue;

		const CommandMessage  input(inputs[i]);
		if (input.getCommand() == CMD_UNKNOWN)
		{
			const string		&prefix = _client->getIsAuthenticated() ? _client->getNickname() : SERVER_NAME;
			string::iterator	begin = inputs[i].begin();
			const string		command_str = get_next_token(begin, inputs[i].end(), ' ');
			ReplyMessage		reply(SERVER_NAME, ERR_UNKNOWNCOMMAND, prefix.c_str(), command_str.c_str(), g_default_replies_map.at(ERR_UNKNOWNCOMMAND), NULL);
			_client->receiveMessage(&reply);
			continue;
		}
		(this->*(_handlers.at(input.getCommand())))(input.getParams());
	}
}

const map<e_commands, EventHandler::CommandHandler> EventHandler::initHandlers(void)
{
	map<e_commands, CommandHandler> handlers;

	handlers[PASS] = &EventHandler::handlePass;
	handlers[NICK] = &EventHandler::handleNick;
	handlers[USER] = &EventHandler::handleUser;
	handlers[JOIN] = &EventHandler::handleJoin;
	handlers[PART] = &EventHandler::handlePart;
	handlers[PRIVMSG] = &EventHandler::handlePrivmsg;
	handlers[QUIT] = &EventHandler::handleQuit;
	handlers[SEND] = &EventHandler::handleSend;

	handlers[KICK] = &EventHandler::handleKick;
	handlers[INVITE] = &EventHandler::handleInvite;
	handlers[TOPIC] = &EventHandler::handleTopic;
	handlers[MODE] = &EventHandler::handleMode;

	return handlers;
}

void EventHandler::handlePass(const vector<string> &args)
{
	if (_client->getIsConnected())
		throw ActionFailedException(ERR_ALREADYREGISTRED, g_default_replies_map.at(ERR_ALREADYREGISTRED), NULL);

	if (args.size() < 1)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "PASS", "usage: PASS <password>", NULL);

	if (hash(args[0]) != _server->getPwdHash())
		throw ActionFailedException(ERR_PASSWDMISMATCH, g_default_replies_map.at(ERR_PASSWDMISMATCH), NULL);
	_client->setIsConnected(true);
}

void EventHandler::handleNick(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getNickname().empty())
		throw ActionFailedException(ERR_ALREADYREGISTRED, g_default_replies_map.at(ERR_ALREADYREGISTRED), NULL);
	if (args.size() < 1)
		throw ActionFailedException(ERR_NONICKNAMEGIVEN, g_default_replies_map.at(ERR_NONICKNAMEGIVEN), NULL);

	const string nickname = to_lower_finnish(args[0]);
	checkNicknameValidity(nickname);
	_client->setNickname(nickname);
	if (!_client->getUsername().empty())
		_client->setAuthenticated(true);
}

void EventHandler::handleUser(const vector<string> &args)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getUsername().empty())
		throw ActionFailedException(ERR_ALREADYREGISTRED, g_default_replies_map.at(ERR_ALREADYREGISTRED), NULL);
	if (args.size() < 4)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "USER", "usage: USER <username> <hostname> <servername> <realname>", NULL);
	_client->setUsername(args[0]);
	(void)args[1]; // args[1] = hostname
	(void)args[2]; // args[2] = servername
	_client->setRealname(args[3]);
	if (!_client->getNickname().empty())
		_client->setAuthenticated(true);
}

void EventHandler::handleJoin(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 1)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "JOIN", "usage: JOIN <channel>{,<channel>} [<key>{,<key>}]", NULL);

	const vector<string>		  channels_to_join = ::split(args[0], ",");
	const map<string, Channel *> &channels = _server->getChannels();
	vector<string>				  keys;

	if (args.size() > 1)
		keys = ::split(args[1], ",");

	for (size_t i = 0; i < channels_to_join.size(); i++)
	{
		const std::map<string, Channel *>::const_iterator channel_it = channels.find(channels_to_join[i]);
		if (channel_it == channels.end())
		{
			const string key = (i < keys.size()) ? keys[i] : "";
			Channel		*new_channel = new Channel(_logger, channels_to_join[i], *_client, key);

			_server->addChannel(*new_channel);
		}
		else
		{
			const string key = (i < keys.size()) ? keys[i] : "";
			Channel		&channel = *channel_it->second;
			_client->joinChannel(channel, key);
		}
	};
}

void EventHandler::handlePart(const vector<string> &args)
{
	const uint16_t n_args = args.size();

	if (n_args < 1)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "PART", "usage: PART <channel>{,<channel>} [<reason>]", NULL);

	const string		 reason = (n_args > 1) ? args[1] : "";
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
		throw ActionFailedException(ERR_NORECIPIENT, "PRIVMSG", "no recipient given (PRIVMSG)", NULL);
	if (n_args < 2)
		throw ActionFailedException(ERR_NOTEXTTOSEND, g_default_replies_map.at(ERR_NOTEXTTOSEND), NULL);

	const CommandMessage msg(_client->getNickname(), PRIVMSG, args[0].c_str(), args[1].c_str(), NULL);
	if (is_channel_prefix(args[0][0]))
	{
		Channel &channel = _server->getChannel(args[0]);
		_client->sendMessage(channel, msg);
	}
	else
	{
		Client &receiver = _server->getClient(args[0]);
		_client->sendMessage(receiver, msg);
	}
}

void EventHandler::handleQuit(const vector<string> &args)
{
	const string				 &reason = args.size() > 0 ? args[0] : "Client quit";
	const string				  quitting_nickname = _client->getNickname();
	const CommandMessage		  quit(quitting_nickname, QUIT, reason.c_str(), NULL);
	const map<string, Channel *> &channels = _client->getChannels();

	for (map<string, Channel *>::const_iterator it_channel = channels.begin(); it_channel != channels.end(); it_channel++)
	{
		const Channel				*channel = it_channel->second;
		const map<string, Client *> &members = channel->getMembers();

		for (map<string, Client *>::const_iterator it_member = members.begin(); it_member != members.end(); it_member++)
		{
			const Client *member = it_member->second;

			if (member && member->getNickname() != quitting_nickname)
				member->receiveMessage(&quit);
		}
	}
	_server->disconnectClient(*_client);
	ostringstream oss;
	oss << "Client " << quitting_nickname << " has quit";
	_logger.logEvent(oss.str());
}

void EventHandler::handleSend(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 2)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "SEND", "usage: SEND <nick|channel> <filename> [filesize]", NULL);

	const string   &sender_ip = _client->getIpAddr();
	const uint16_t &sender_port = getRandomPort();
	const string   &filename = args[1];
	const uint32_t	file_size = args.size() > 2 ? std::atol(args[2].c_str()) : 0;
	const uint64_t	ip_long = ip_to_long(sender_ip);
	ostringstream	dcc_send;

	dcc_send << "DCC SEND " << filename << " " << ip_long << " " << sender_port << " " << file_size;
	const CommandMessage msg(_client->getNickname(), PRIVMSG, args[0].c_str(), dcc_send.str().c_str(), NULL);

	if (is_channel_prefix(args[0][0]))
	{
		Channel &channel = _server->getChannel(args[0]);
		_client->sendMessage(channel, msg);
	}
	else
	{
		Client &receiver = _server->getClient(args[0]);
		_client->sendMessage(receiver, msg);
	}
}

void EventHandler::handleKick(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t n_args = args.size();
	if (n_args < 2)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "KICK", "usage: KICK <channel> <nickname> [<reason>]", NULL);

	Channel &channel = _server->getChannel(args[0]);
	Client	&target = _server->getClient(args[1]);

	args.size() > 2 ? _client->kick(target, channel, args[2]) : _client->kick(target, channel);
}

void EventHandler::handleInvite(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 2)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "INVITE", "usage: INVITE <nickname> <channel>", NULL);

	Client	&target = _server->getClient(args[0]);
	Channel &channel = _server->getChannel(args[1]);

	_client->invite(target, channel);
}

void EventHandler::handleTopic(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const size_t n_args = args.size();

	if (n_args < 1)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "TOPIC", "usage: TOPIC <channel> [<topic>]", NULL);

	Channel &channel = _client->getChannel(args[0]);
	if (n_args == 1)
	{
		const string &topic = channel.getTopic();
		const string &channel_name = channel.getName();
		const string &issuer_nickname = _client->getNickname();
		ReplyMessage  topic_reply;

		if (topic.empty())
			topic_reply = ReplyMessage(SERVER_NAME, RPL_NOTOPIC, issuer_nickname.c_str(), channel_name.c_str(), g_default_replies_map.at(RPL_NOTOPIC), NULL);
		else
			topic_reply = ReplyMessage(SERVER_NAME, RPL_TOPIC, issuer_nickname.c_str(), channel_name.c_str(), topic.c_str(), NULL);
		_client->receiveMessage(&topic_reply);
	}
	else
		_client->topicSet(channel, args[1]);
}

void EventHandler::handleMode(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	const uint16_t n_args = args.size();

	if (n_args < 1)
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}", NULL);

	Channel &channel = _server->getChannel(args[0]);

	if (n_args == 1)
	{
		const map<char, bool> &modes = channel.getModes();
		string				   modes_str;

		for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second)
				modes_str += it->first;
		}
		if (!modes_str.empty())
			modes_str.insert(0, "+");
		const ReplyMessage channel_mode(SERVER_NAME, RPL_CHANNELMODEIS, _client->getNickname().c_str(), channel.getName().c_str(), modes_str.c_str(), NULL);
		_client->receiveMessage(&channel_mode);
		return;
	}

	if (args[1][0] != '+' && args[1][0] != '-')
		throw ActionFailedException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <target> {[+|-]<modes> [<mode_params>]}", NULL);

	bool			status;
	uint16_t		param_index = 2;
	map<char, bool> new_modes;
	vector<string>	params;

	params.reserve(n_args - 2);
	for (string::const_iterator mode = args[1].begin(); mode != args[1].end(); mode++)
	{
		if (*mode == '+' || *mode == '-')
		{
			status = (*mode == '+');
			continue;
		}
		if (!is_valid_channel_mode(*mode))
			throw ActionFailedException(ERR_UNKNOWNMODE, string(1, *mode).c_str(), NULL);
		if (channel_mode_requires_param(*mode, status))
		{
			if (param_index >= n_args)
				throw ActionFailedException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <channel> <mode> [<param>]", NULL);
			params.push_back(args[param_index++]);
		}
		new_modes[*mode] = status;
	}
	_client->modesChange(channel, new_modes, params);
}

void EventHandler::checkNicknameValidity(const string &nickname) const
{
	if (nickname.empty())
		throw ActionFailedException(ERR_NONICKNAMEGIVEN, g_default_replies_map.at(ERR_NONICKNAMEGIVEN), NULL);
	if (!is_valid_nickname(nickname))
	{
		const string nickname_rules_msg = "allowed characters: A-Z, a-z, 0-9, -, [, ], \\, `, ^, {, }\nmax nickname lenght: " + to_string(static_cast<int>(MAX_NICKNAME_LEN));
		throw ActionFailedException(ERR_ERRONEOUSNICKNAME, nickname.c_str(), nickname_rules_msg.c_str(), NULL);
	}
	if (_server->isClientConnected(nickname))
		throw ActionFailedException(ERR_NICKNAMEINUSE, nickname.c_str(), g_default_replies_map.at(ERR_NICKNAMEINUSE), NULL);
}

uint16_t EventHandler::getRandomPort(void) const
{
	return 1024 + (rand() % (65535 - 1024));
}

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw ActionFailedException(ERR_NOTREGISTERED, "you are not connected, use PASS <password> first", NULL);
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw ActionFailedException(ERR_NOTREGISTERED, "you are not registered, use NICK <nickname> and USER <username> first", NULL);
}
