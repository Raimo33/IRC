/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 12:23:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/EventHandler.hpp"
#include "irc/Server.hpp"
#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/Message.hpp"
#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"

#include <algorithm>

using std::string;
using std::map;
using std::vector;

static void		checkConnection(const Client *client);
static void		checkAuthentication(const Client *client);

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
			const Message input(cmds[i]);
			(this->*(_handlers.at(static_cast<e_commands>(input.getValue()))))(input.getParams());
		}
		catch (ProtocolErrorException &e)
		{
			Message &reply = e.getContent();
			if (reply.getValue() == ERR_UNKNOWNCOMMAND)
			{
				if (_client->getIsAuthenticated())
					reply.setParam(_client->getNickname(), 0);
				else
					reply.setParam(SERVER_NAME, 0);
				_client->receiveMessage(reply);
				_logger.logError(&e);
			}
			else
				throw ;
		}
	}
}

void	EventHandler::sendBufferedMessage(const Client &receiver, const Message &message)
{
	string		first_part, second_part, to_send;
	uint16_t	block_size, send_length;
	const int	receiver_socket = receiver.getSocket();

	unwrapMessage(message, &first_part, &second_part);
	block_size = BUFFER_SIZE - first_part.size() - 2; //2 per \r\n
	while (!second_part.empty())
	{
		send_length = std::min(static_cast<size_t>(block_size),  second_part.size());
		to_send = first_part + second_part.substr(0, send_length) + "\r\n";
		std::cout << "Sending: " << to_send << std::endl;
		second_part = second_part.substr(send_length); // Update second_part correctly
		send_p(receiver_socket, to_send.c_str(), to_send.length(), 0);
	}
}

void	EventHandler::unwrapMessage(const Message &msg, string *first_part, string *second_part)
{
	const uint16_t			&value = msg.getValue();
	const string			&prefix = msg.getPrefix();
	const vector<string>	&params = msg.getParams();
	const bool is_reply = value >= RPL_WELCOME && value <= ERR_CHANOPRIVSNEEDED;

	if (!prefix.empty())
		*first_part += ":" + prefix + " ";
	if (is_reply)
	{
		ostringstream oss;
		oss << std::setw(3) << std::setfill('0') << value;
		*first_part += oss.str();
	}
	else
		*first_part += _command_strings.at(value);
	for (vector<string>::const_iterator it = params.begin(); it != params.end() - 1; it++)
		*second_part += " " + *it;
	if (!params.empty())
		*second_part += " :" + params.back();
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
	handlers[SEND] = &EventHandler::handleSend;

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
		command_strings[SEND] = "SEND";

		command_strings[KICK] = "KICK";
		command_strings[INVITE] = "INVITE";
		command_strings[TOPIC] = "TOPIC";
		command_strings[MODE] = "MODE";
	}
	return command_strings;
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

	const Message msg(_client->getNickname(), PRIVMSG, args[0].c_str(), args[1].c_str(), NULL);
	if (is_channel_prefix(args[0][0])) //se il primo carattere e' #, &, + o !
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
	const string					&reason = args.size() > 0 ? args[0] : "Client quit";
	const string					&quitting_nickname = _client->getNickname();
	const Message			quit(quitting_nickname, QUIT, reason.c_str(), NULL);
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
	ostringstream oss;
	oss << "Client " << quitting_nickname << " has quit";
	_logger.logEvent(oss.str());
}

void EventHandler::handleSend(const vector<string> &args)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (args.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "SEND", "usage: SEND <nick|channel> <filename> [filesize]", NULL);

	const string		&sender_ip = _client->getIpAddr();
	const uint16_t		&sender_port = getRandomPort();
	const string		&filename = args[1];
	const uint32_t		file_size = args.size() > 2 ? std::atol(args[2].c_str()) : 0;
	const uint64_t		ip_long = ip_to_long(sender_ip);
	ostringstream		dcc_send;

	dcc_send << "DCC SEND " << filename << " " << ip_long << " " << sender_port << " " << file_size;
	const Message msg(_client->getNickname(), PRIVMSG, args[0].c_str(), dcc_send.str().c_str(), NULL);

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
		Message	topic_reply;

		if (topic.empty())
			topic_reply = Message(SERVER_NAME, RPL_NOTOPIC, issuer_nickname.c_str(), channel_name.c_str(), default_replies.at(RPL_NOTOPIC), NULL);
		else
			topic_reply = Message(SERVER_NAME, RPL_TOPIC, issuer_nickname.c_str(), channel_name.c_str(), topic.c_str(), NULL);
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
		const Message	reply(SERVER_NAME, RPL_CHANNELMODEIS, _client->getNickname().c_str(), channel.getName().c_str(), modes_str.c_str(), NULL);
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

uint16_t	EventHandler::getRandomPort(void) const
{
	return 1024 + (rand() % (65535 - 1024));
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
