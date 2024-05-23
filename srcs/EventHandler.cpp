/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 14:01:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/EventHandler.hpp"
#include "irc/Server.hpp"
#include "irc/PrivateMessage.hpp"
#include "irc/Message.hpp"
#include "irc/Channel.hpp"
#include "irc/ChannelOperator.hpp"
#include "irc/Hasher.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/ReplyCodes.hpp"

#include "irc/Exceptions.hpp"

using namespace std;
using namespace irc;

static void checkConnection(const Client *client);
static void checkAuthentication(const Client *client);

EventHandler::EventHandler(void) :
	_server(NULL),
	_client(NULL),
	_commands(initCommands())
{
	initHandlers();
}

EventHandler::EventHandler(Server *server) :
	_server(server),
	_client(NULL),
	_commands(initCommands())
{
	initHandlers();
}

EventHandler::EventHandler(const EventHandler &copy) :
	_server(copy._server),
	_client(copy._client),
	_commands(copy._commands)
{
	initHandlers();
}
	
EventHandler::~EventHandler(void) {}

const map<string, e_cmd_type>	&EventHandler::getCommands(void) const
{
	return _commands;
}

const Client	*EventHandler::getClient(void) const
{
	return _client;
}

void	EventHandler::setClient(Client *client)
{
	_client = client;
}

const Server	*EventHandler::getServer(void) const
{
	return _server;
}

void	EventHandler::setServer(Server *server)
{
	_server = server;
}

//JOIN #channel1,#channel2,#channel3 key1,key2,key3
void	EventHandler::processInput(string raw_input)
{
	s_message input = parseInput(raw_input);

    if (input.command < 0 || input.command >= N_COMMANDS)
        throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, input.command);

    (this->*(_handlers[input.command]))(input.params);
}

void	EventHandler::initHandlers(void)
{
	_handlers[PRIVMSG] = &EventHandler::handlePrivmsg;
	_handlers[JOIN] = &EventHandler::handleJoin;
	_handlers[PASS] = &EventHandler::handlePass;
	_handlers[NICK] = &EventHandler::handleNick;
	_handlers[QUIT] = &EventHandler::handleQuit;

	_handlers[KICK] = &EventHandler::handleKick;
	_handlers[INVITE] = &EventHandler::handleInvite;
	_handlers[TOPIC] = &EventHandler::handleTopic;
	_handlers[MODE] = &EventHandler::handleMode;
}

const std::map<std::string, e_cmd_type>	EventHandler::initCommands(void)
{
	static map<string, e_cmd_type>	commands;

	if (!commands.empty()) //se il map e' gia' stato inizializzato
		return commands;

	commands["PRIVMSG"] = PRIVMSG;
	commands["JOIN"] = JOIN;
	commands["MODE"] = MODE;
	commands["PASS"] = PASS;
	commands["NICK"] = NICK;
	commands["USER"] = USER;
	commands["QUIT"] = QUIT;
	// commands["TOPIC"] = TOPIC;

	return commands;
}

void	EventHandler::sendBufferedMessage(const Client &receiver, const struct s_messageBase &message)
{
	string	full_msg;

	if (!message.prefix.empty())
		full_msg += ":" + message.prefix + " ";

	//TODO tramite il dynamic cast capire il tipo di messaggio e costruire il messaggio accordingly
	//TODO implementare (deve ripetere il prefix e i parametri ogni volta)
}

//input: ":<prefix> <command> <params> <crlf>"
s_message	EventHandler::parseInput(string &raw_input) const
{
	s_message	input;
	string	command;

	raw_input = raw_input.substr(0, MAX_MSG_LENGTH - 1); //limito la lunghezza dell'input (per evitare buffer overflow

	if (raw_input.size() >= 2 && raw_input.substr(raw_input.size() - 2) == "\r\n")
		raw_input = raw_input.substr(0, raw_input.size() - 2);

	input.prefix = "";
	if (raw_input[0] == ':')
	{
		input.prefix = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il prefix
		raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il prefix
	}
	command = raw_input.substr(0, raw_input.find(' ') - 1); //prendo il comando come stringa

	if (_commands.find(command) == _commands.end()) //se il comando non esiste
		throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command);
	input.command = _commands.at(command); //associo il comando all'enum

	raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il comando
	string param;
	while (raw_input.find(' ') != string::npos) //finchè ci sono parametri
	{
		if (raw_input[0] == ':') //se il parametro inizia con ':' allora tutto ciò che segue è il parametro (anche spazi, caratteri speciali)
		{
			param = raw_input.substr(1); //prendo tutto il resto apparte il ':'
			input.params.push_back(param);
			break;
		}
		param = raw_input.substr(0, raw_input.find(' ')); //prendo il parametro
		input.params.push_back(param); //aggiungo il parametro al vettore
		raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il parametro
	}
	return input;
}

void EventHandler::handlePrivmsg(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (params.size() < 1)
		throw ProtocolErrorException(ERR_NORECIPIENT, "PRIVMSG"); //TODO aggiungere la reason (No recipient given (PRIVMSG))
	if (params.size() < 2)
		throw ProtocolErrorException(ERR_NOTEXTTOSEND);

	if (is_channel_prefix(params[0][0])) //se il primo carattere e' #, &, + o !
	{
		//channel msg PRIVMSG <channel> :<message>

		Channel channel = _server->getChannel(params[0]);
		int		n_members = channel.getMembers().size();

		if (n_members <= 2)
		{
			//promuovo il messaggio a private message
			Client	*receiver;

			receiver = channel.getMembers().begin()->second;
			if (receiver->getNickname() == _client->getNickname())
				receiver = channel.getMembers().rbegin()->second;
			PrivateMessage *msg = new PrivateMessage(params[1], *_client, *receiver);
			_client->sendMessage(*receiver, *msg);
			delete msg;
			return ;
		}
		Message *msg = new Message(params[1], *_client, channel);
		_client->sendMessage(channel, *msg);
		delete msg;
	}
	else
	{
		//private msg PRIVMSG <nickname> :<message>
		Client receiver = _server->getClient(params[0]);

		PrivateMessage *msg = new PrivateMessage(params[1], *_client, receiver);
		_client->sendMessage(receiver, *msg);
		delete msg;
	}
}

//JOIN <channel1,channel2,channel3> <key1,key2,key3>
void EventHandler::handleJoin(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);
	if (params.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "JOIN"); //TODO aggiungere la reason (usage: JOIN <channel> <key>)

	vector<string> channels = split(params[0], ',');
	vector<string> keys = split(params[1], ',');

	for (size_t i = 0; i < channels.size(); i++)
	{
		try
		{
			Channel	channel = _server->getChannel(channels[i]);
			if (i < keys.size())
				_client->joinChannel(channel, keys[i]);
			else
				_client->joinChannel(channel);
		}
		catch (ProtocolErrorException &e)
		{
			if (e.getCode() == ERR_NOSUCHCHANNEL)
			{
				ChannelOperator op(*_client);

				if (i < keys.size())
					_server->addChannel(new Channel(channels[i], keys[i], op));
				else
					_server->addChannel(new Channel(channels[i], op));
				_client->joinChannel(_server->getChannel(channels[i]));
			}
			else
				throw e;
		}
	};
}

void EventHandler::handlePass(const vector<string> &params)
{
	if (_client->getIsConnected())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);

	if (params.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PASS"); //TODO aggiungere la reason (usage: PASS <password>)

	if (Hasher::hash(params[0]) != _server->getPwdHash())
		throw ProtocolErrorException(ERR_PASSWDMISMATCH);
	_client->setIsConnected(true);
}

void EventHandler::handleNick(const vector<string> &params)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getNickname().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);
	if (params.size() < 1)
		throw ProtocolErrorException(ERR_NONICKNAMEGIVEN);
	checkNicknameValidity(params[0]);
	_client->setNickname(params[0]);
	if (!_client->getUsername().empty())
		_client->setAuthenticated(true);
}

//TODO gestire <hostname> <servername> <realname>
void EventHandler::handleUser(const vector<string> &params)
{
	checkConnection(_client);
	if (_client->getIsAuthenticated() || !_client->getUsername().empty())
		throw ProtocolErrorException(ERR_ALREADYREGISTRED);
	if (params.size() < 1)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "USER"); //TODO aggiungere la reason (usage: USER <username> <hostname> <servername> <realname>)
	_client->setUsername(params[0]);
	if (!_client->getNickname().empty())
		_client->setAuthenticated(true);
}

void EventHandler::handleQuit(const vector<string> &params)
{
	(void)params;
	_server->removeClient(*_client);
}

//per i comandi da operator, il checkPrivilege viene gia' fatto in ChannelOperator

void EventHandler::handleKick(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (params.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "KICK"); //TODO aggiungere la reason (usage: KICK <channel> <nickname> <reason>

	//TODO imlementare
}

void EventHandler::handleInvite(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (params.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "INVITE"); //TODO aggiungere la reason (usage: INVITE <nickname> <channel>)
	//TODO implementare
}

void EventHandler::handleTopic(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);

	//TODO implementare
	//(RPL_TOPIC)
}

void EventHandler::handleMode(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);

	if (params.size() < 2)
		throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE"); //TODO aggiungere la reason (usage: MODE <channel> <mode>)
	//TODO implementare
	//(RPL_CHANNELMODEIS)
	//(RPL_TOPIC_WHO_TIME)
	//(RPL_TOPIC)
	//(ERR_UNKNOWNMODE)
	(void)params;
}

void	EventHandler::checkNicknameValidity(const string &nickname) const
{
	if (!is_valid_nickname(nickname))
		throw ProtocolErrorException(ERR_ERRONEOUSNICKNAME, nickname); //TODO aggiungere la reason (allowed characters: A-Z, a-z, 0-9, -, [, ], \, `, ^, {, }, MAX_NICKNAME_LEN)
	if (_server->isClientConnected(nickname))
		throw ProtocolErrorException(ERR_NICKNAMEINUSE, nickname);
}

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw ProtocolErrorException(ERR_NOTREGISTERED); //TODO aggiungere la reason (you are not connected, use PASS <password> first)
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOTREGISTERED); //TODO aggiungere la reason (you are not registered, use NICK <nickname> and USER <username> first)
}
