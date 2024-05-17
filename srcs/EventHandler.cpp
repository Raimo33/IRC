/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 18:28:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/EventHandler.hpp"
#include "headers/Server.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Message.hpp"
#include "headers/Channel.hpp"

EventHandler::EventHandler(void) :
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

EventHandler::EventHandler(Client *client, Server *server) :
	_client(client),
	_server(server)
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

EventHandler::~EventHandler(void) {}

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
			executeCommandQuit(input.params);
			break;
		default:
			throw UnknownCommandException();
	}
}

void EventHandler::setClient(Client *client)
{
	_client = client;
}

void EventHandler::setServer(Server *server)
{
	_server = server;
}

Client	&EventHandler::getClient(void) const
{
	return *_client;
}

Server	&EventHandler::getServer(void) const
{
	return *_server;
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
			User receiver;

			receiver = *channel.getUsers().begin()->second;
			if (receiver.getNickname() == _client->getNickname())
				receiver = *channel.getUsers().rbegin()->second;
			if (receiver.getNickname() == _client->getNickname())
				throw Server::CantSendMessageToYourselfException();
			PrivateMessage *msg = new PrivateMessage(params[2], *_client, receiver);
			deliverMessage(receiver, *msg);
			return ;
		}
		Message *msg = new Message(params[1], *_client, channel);
		deliverMessage(channel, *msg);
	}
	else
	{
		//private msg PRIVMSG <nickname> :<message>
		User receiver = _server->getUser(params[0]);
		PrivateMessage *msg = new PrivateMessage(params[1], *_client, receiver);
		deliverMessage(receiver, *msg);
	}
}

void EventHandler::executeCommandMode(const vector<string> &params)
{
	//TODO
	(void)params;
}

void EventHandler::executeCommandJoin(const vector<string> &params)
{
	//JOIN <channel1,channel2,channel3> <key1,key2,key3>
	vector<string> channels = split(params[0], ','); //se non e' in cpp98 mettiamolo in utils
	vector<string> keys = split(params[1], ',');

	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel channel;

		try
		{
			channel = _server->getChannel(channels[i]);
		}
		catch (Server::ChannelNotFoundException &e)
		{
			//TODO create channel (aggiungerlo alla lista di canali del server)
			//TODO capire la questione delle chiavi, se il canale nuovo deve essere inizializzato con la chiave nel caso in cui l'utente la scriva
		}
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

void EventHandler::executeCommandQuit(const vector<string> &params)
{
	(void)params;
	_server->removeClient(_client);
}

void EventHandler::executeCommandUser(const vector<string> &params)
{
	//se non e' un username gia' in uso
	_client->setUsername(params[0]);
	_client->authenticate();
	//TODO aggiungere l'utente alla lista di utenti del server
	//TODO aggiungere le credenziali dell'user alla lista di credenziali del server
}

void	EventHandler::deliverMessage(const Channel &channel, const Message &msg) const
{
    std::map<std::string, User*> users = channel.getUsers();

    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        sendBufferedString(*it->second, msg.getContent());
    }
}

void	EventHandler::deliverMessage(const User &receiver, const PrivateMessage &msg) const
{
	sendBufferedString(receiver, msg.getContent());
}

void	EventHandler::sendBufferedString(const User &receiver, const string &string) const
{
	const char		*raw_msg = string.c_str();
	int				socket = _server->getClient(receiver).getSocket();
	const size_t	total_len = strlen(raw_msg);
	size_t			chars_sent = 0;

	while (chars_sent < total_len)
	{
		size_t len = min(total_len - chars_sent, BUFFER_SIZE - 1);
		send(socket, raw_msg + chars_sent, len, 0);
		chars_sent += len;
	}
}

const char *EventHandler::UnknownCommandException::what() const throw()
{
	return "Unknown command";
}
