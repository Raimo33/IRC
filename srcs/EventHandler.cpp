/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 03:03:53 by craimond         ###   ########.fr       */
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
	s_input input = parseInput(raw_input);

    if (input.command < 0 || input.command >= N_COMMANDS)
        throw UnknownCommandException();

    (this->*(_handlers[input.command]))(input.params);
}

void	EventHandler::initHandlers(void)
{
	_handlers[PRIVMSG] = &EventHandler::handlePrivmsg;
	_handlers[JOIN] = &EventHandler::handleJoin;
	_handlers[MODE] = &EventHandler::handleMode;
	_handlers[PASS] = &EventHandler::handlePass;
	_handlers[NICK] = &EventHandler::handleNick;
	_handlers[QUIT] = &EventHandler::handleQuit;
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

	return commands;
}

//input: ":<prefix> <command> <params> <crlf>"
s_input	EventHandler::parseInput(string &raw_input) const
{
	s_input	input;
	string	command;

	raw_input = raw_input.substr(0, MAX_INPUT_LEN - 1); //limito la lunghezza dell'input (per evitare buffer overflow

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
		throw UnknownCommandException();
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
	if (is_channel_prefix(params[0][0])) //se il primo carattere e' #, &, + o !
	{
		//channel msg PRIVMSG <channel> :<message>

		Channel channel = _server->getChannel(params[0]);
		int		n_members = channel.getMembers().size();

		if (n_members == 1)
			throw CantSendMessageToYourselfException();
		if (n_members == 2)
		{
			//promuovo il messaggio a private message
			Client	*receiver;

			receiver = channel.getMembers().begin()->second;
			if (receiver->getUsername() == _client->getUsername())
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

void EventHandler::handleMode(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);
	//TODO implementare
	(void)params;
}

//JOIN <channel1,channel2,channel3> <key1,key2,key3>
void EventHandler::handleJoin(const vector<string> &params)
{
	checkConnection(_client);
	checkAuthentication(_client);
	vector<string> channels = split(params[0], ','); //se non e' in cpp98 mettiamolo in utils
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
		catch (ChannelNotFoundException &e)
		{
			ChannelOperator op(*_client);

			if (i < keys.size())
				_server->addChannel(new Channel(channels[i], keys[i], op));
			else
				_server->addChannel(new Channel(channels[i], op));
		}
	};
}

void EventHandler::handlePass(const vector<string> &params)
{
	if (_client->getIsConnected())
		throw AlreadyConnectedException();

	Hasher hasher(params[0]);

	if (hasher.hexdigest() != _server->getPwdHash())
		throw InvalidPasswordException();
	_client->setIsConnected(true);
}

void EventHandler::handleNick(const vector<string> &params)
{
	checkConnection(_client);
	try
	{
		checkNicknameValidity(params[0]);
		_client->setNickname(params[0]);
		if (!_client->getUsername().empty())
			_client->setAuthenticated(true);
	}
	catch (ErroneousNicknameException &e)
	{
		_client->receiveNumericReply(ERR_ERRONEOUSNICKNAME, vector<string>(1, params[0]));
	}
	catch (NicknameInUseException &e)
	{
		_client->receiveNumericReply(ERR_NICKNAMEINUSE, vector<string>(1, params[0]));
	}
}

void EventHandler::handleQuit(const vector<string> &params)
{
	(void)params;
	_server->removeClient(*_client);
}

void EventHandler::handleUser(const vector<string> &params)
{
	checkConnection(_client);
	_client->setUsername(params[0]);
	if (!_client->getNickname().empty())
		_client->setAuthenticated(true);
}

void	EventHandler::sendBufferedString(const Client &client, const string &string)
{
	const char		*raw_msg = string.c_str();
	int				socket = client.getSocket();
	const size_t	total_len = strlen(raw_msg);
	size_t			chars_sent = 0;

	while (chars_sent < total_len)
	{
		size_t len = min(total_len - chars_sent, BUFFER_SIZE - 1);
		send(socket, raw_msg + chars_sent, len, 0); //TODO gestire errori, provare a inviare di nuovo (mettere in un file di log)
		chars_sent += len;
	}
}

void	EventHandler::checkNicknameValidity(const string &nickname) const
{
	try
	{
		_server->getClient(nickname);
		throw NicknameInUseException();
	}
	catch (ClientNotFoundException &e)
	{
		(void)e;
	}
	if (!is_valid_nickname(nickname))
		throw ErroneousNicknameException();
}

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw NotConnectedException();
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw NotAuthenticatedException();
}
