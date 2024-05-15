/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/15 16:31:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/EventHandler.hpp"
#include "headers/Server.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Message.hpp"
#include "headers/Channel.hpp"

EventHandler::EventHandler() :
	_client(NULL),
	_server(NULL)
{
	_commands["PRIVMSG"] = PRIVMSG;
	_commands["JOIN"] = JOIN;
	_commands["MODE"] = MODE;
	_commands["PASS"] = PASS;
	_commands["NICK"] = NICK;
	_commands["USER"] = USER;
	_commands["QUIT"] = QUIT;
}

EventHandler::EventHandler(const EventHandler &copy) :
	_commands(copy._commands),
	_client(copy._client),
	_server(copy._server) {}

EventHandler::~EventHandler() {}

EventHandler	&EventHandler::operator=(const EventHandler &rhs)
{
	if (this != &rhs)
	{
		_commands = rhs._commands;
		_client = rhs._client;
		_server = rhs._server;
	}
	return *this;
}

//input: ":<prefix> <command> <params> <crlf>"
t_input	EventHandler::parseInput(string &raw_input) const
{
	t_input	input;
	string	command;

	input.prefix = "";
	if (raw_input[0] == ':')
		input.prefix = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il prefix
	raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il prefix
	command = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il comando come stringa

	map<string, t_cmd>::const_iterator it;

	it = _commands.find(command);
	if (it == _commands.end())
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

//JOIN #channel1,#channel2,#channel3 key1,key2,key3
void	EventHandler::processInput(string raw_input)
{
	t_input	input = parseInput(raw_input);
	switch (input.command)
	{
		case PRIVMSG: //usato sia per messaggi privati che per messaggi nei canali
			_client->checkConnection();
			executeCommandPrivmsg(input.params);
			break;
		//JOIN <channel1,channel2,channel3> <key1,key2,key3>
		case JOIN:
			_client->checkConnection();
			executeCommandJoin(input.params);
			break;
		case MODE:
			_client->checkConnection();
			//TODO
			break;
		//PASS <connectionpassword>
		case PASS: //viene usata dagli utenti per autenticarsi
			executeCommandPass(input.params);
			break;
		//NICK <nickname>
		case NICK:
			if (!_client->getIsConnected())
				throw Client::NotConnectedException();
			executeCommandNick(input.params);
			break;
		//USER <username>
		case USER:
			if (!_client->getIsConnected())
				throw Client::NotConnectedException();
			executeCommandUser(input.params);
			break;
		//QUIT
		case QUIT:
			//TODO forse va tolto il client anche da tutti i canali di cui fa parte
			_server->removeClient(_client);
			break;
		default:
			throw UnknownCommandException();
	}
}

void EventHandler::executeCommandPrivmsg(const vector<string> &params)
{
	if (is_channel_prefix(params[0][0])) //se il primo carattere e' #, &, + o !
	{
		//channel msg PRIVMSG <channel> :<message>
		Channel channel = _server->getChannel(params[0]);
		if (channel.getMembersCount() <= 2)
		{
			//promuovo il messaggio a private message
			User receiver = channel.getMember(params[1]);
			PrivateMessage *msg = new PrivateMessage(params[2], *_client, receiver);
			_client->sendMessage(receiver, *msg);
			return ;
		}
		Message *msg = new Message(params[1], *_client, channel);
		_client->sendMessage(channel, *msg);
	}
	else
	{
		//private msg PRIVMSG <nickname> :<message>
		User receiver = _server->getClient(params[0]);
		PrivateMessage *msg = new PrivateMessage(params[1], *_client, receiver);	
		_client->sendMessage(receiver, *msg);
	}
}

void EventHandler::executeCommandJoin(const vector<string> &params)
{
	//JOIN <channel1,channel2,channel3> <key1,key2,key3>
	vector<string> channels = split(params[0], ','); //se non e' in cpp98 mettiamolo in utils
	vector<string> keys = split(params[1], ',');

	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel channel = _server->getChannel(channels[i]);
		if (i < keys.size())
			_client->joinChannel(channel, keys[i]);
		else
			_client->joinChannel(channel);
	};
}

void EventHandler::executeCommandPass(const vector<string> &params)
{
	if (_client->getIsConnected())
		throw Client::AlreadyConnectedException();
	if (Hasher::hash(params[0]) != _server->getPwdHash())
		throw Client::InvalidPasswordException();
	_client->setIsConnected(true);
}

void EventHandler::executeCommandNick(const vector<string> &params)
{
	//se non e' un nickname gia' in uso
	_client->setNickname(params[0]);
}

void EventHandler::executeCommandUser(const vector<string> &params)
{
	//se non e' un username gia' in uso
	_client->setUsername(params[0]);
	_client->authenticate();
}

const char *EventHandler::UnknownCommandException::what() const throw()
{
	return "Unknown command";
}
