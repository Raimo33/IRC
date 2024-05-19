/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:21:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/EventHandler.hpp"
#include "headers/Server.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Message.hpp"
#include "headers/Channel.hpp"
#include "headers/ChannelOperator.hpp"

static void checkConnection(const Client *client);
static void checkAuthentication(const Client *client);

EventHandler::EventHandler(Client *client, Server *server) :
	_commands(initCommandMap()),
	_client(client),
	_server(server) {}

EventHandler::~EventHandler(void) {}

const map<string, e_cmd_type>	EventHandler::getCommands(void) const
{
	return _commands;
}

const Client	*EventHandler::getClient(void) const
{
	return _client;
}

const Server	*EventHandler::getServer(void) const
{
	return _server;
}

//JOIN #channel1,#channel2,#channel3 key1,key2,key3
void	EventHandler::processInput(string raw_input)
{
	s_input	input = parseInput(raw_input);

	switch (input.command)
	{
		case PRIVMSG: //usato sia per messaggi privati che per messaggi nei canali
			checkConnection(_client);
			checkAuthentication(_client);
			executeCommandPrivmsg(input.params);
			break;
		//JOIN <channel1,channel2,channel3> <key1,key2,key3>
		case JOIN:
			checkConnection(_client);
			checkAuthentication(_client);
			executeCommandJoin(input.params);
			break;
		case MODE:
			checkConnection(_client);
			checkAuthentication(_client);
			//TODO mode
			break;
		//PASS <connectionpassword>
		case PASS: //viene usata dagli utenti per autenticarsi
			executeCommandPass(input.params);
			break;
		//NICK <nickname>
		case NICK:
			checkConnection(_client);
			executeCommandNick(input.params);
			break;
		//USER <username>
		case USER:
			checkConnection(_client);
			executeCommandUser(input.params);
			break;
		//QUIT
		case QUIT:
			executeCommandQuit(input.params);
			break;
		default:
			throw CommandNotFoundException();
	}
}

void	EventHandler::deliverMessage(const Channel &channel, const Message &msg) const
{
    std::map<std::string, User*> users = channel.getMembers();

    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it)
        sendBufferedString(*it->second, msg.getContent());
}

void	EventHandler::deliverMessage(const User &receiver, const PrivateMessage &msg) const
{
	if (receiver.getNickname() == _client->getNickname())
		throw CantSendMessageToYourselfException();
	sendBufferedString(receiver, msg.getContent());
}

//input: ":<prefix> <command> <params> <crlf>"
s_input	EventHandler::parseInput(string &raw_input) const
{
	s_input	input;
	string	command;
	
	if (raw_input.size() >= 2 && raw_input.substr(raw_input.size() - 2) == "\r\n")
        raw_input = raw_input.substr(0, raw_input.size() - 2);

	input.prefix = "";
	if (raw_input[0] == ':')
		input.prefix = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il prefix
	raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il prefix
	command = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il comando come stringa

	map<string, e_cmd_type>::const_iterator it;

	it = _commands.find(command);
	if (it == _commands.end())
		throw CommandNotFoundException();
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

void EventHandler::executeCommandPrivmsg(const vector<const string> &params)
{
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
			User *receiver;

			receiver = channel.getMembers().begin()->second;
			if (receiver->getNickname() == _client->getNickname())
				receiver = channel.getMembers().rbegin()->second;
			PrivateMessage *msg = new PrivateMessage(params[2], *_client, *receiver);
			deliverMessage(*receiver, *msg);
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
		try
		{
			Channel	channel = _server->getChannel(channels[i]);
			if (i < keys.size())
				_client->joinChannel(channel, keys[i]);
			else
				_client->joinChannel(channel);
		}
		catch (Server::ChannelNotFoundException &e)
		{
			ChannelOperator op(*_client);

			if (i < keys.size())
				_server->addChannel(Channel(channels[i], keys[i], op));
			else
				_server->addChannel(Channel(channels[i], op));
		}
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
	//TODO se non e' un nickname gia' in uso
	_client->setNickname(params[0]);
}

void EventHandler::executeCommandQuit(const vector<string> &params)
{
	(void)params;
	_server->removeClient(_client);
}

void EventHandler::executeCommandUser(const vector<string> &params)
{
	_client->setUsername(params[0]);
	_client->authenticate();
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
		send(socket, raw_msg + chars_sent, len, 0); //TODO gestire errori, provare a inviare di nuovo (mettere in un file di log)
		chars_sent += len;
	}
}

const map<string, e_cmd_type>	&EventHandler::initCommandMap(void) const
{
	static map<string, e_cmd_type>	commands;

	if (!commands.empty()) //se il map e' gia' stato inizializzato
		return commands;
	commands["PRIVMSG"] = CMD_PRIVMSG;
	commands["JOIN"] = CMD_JOIN;
	commands["MODE"] = CMD_MODE;
	commands["PASS"] = CMD_PASS;
	commands["NICK"] = CMD_NICK;
	commands["USER"] = CMD_USER;
	commands["QUIT"] = CMD_QUIT;
	return commands;
}

static void checkConnection(const Client *client)
{
	if (!client->getIsConnected())
		throw Client::NotConnectedException();
}

static void checkAuthentication(const Client *client)
{
	if (!client->getIsAuthenticated())
		throw User::NotAuthenticatedException();
}

const char	*EventHandler::CommandNotFoundException::what(void) const throw()
{
	return "Command not found";
}

const char	*EventHandler::CantSendMessageToYourselfException::what(void) const throw()
{
	return "You can't send a message to yourself";
}