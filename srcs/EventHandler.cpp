/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 12:26:00 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/EventHandler.hpp"

EventHandler::EventHandler() {}

EventHandler::EventHandler(const EventHandler &copy) {}

EventHandler::~EventHandler() {}

EventHandler	&EventHandler::operator=(const EventHandler &rhs)
{
	return *this;
}
void	EventHandler::processInput(string raw_input, const Client &client)
{
	t_input	input = parseInput(raw_input);
	switch (input.command)
	{
		case PRIVMSG: //usato sia per messaggi privati che per messaggi nei canali
			client.checkConnection();
			if (is_channel_prefix(input.params[0])) //se il primo carattere e' #, &, + o !
			{
				//channel msg PRIVMSG <channel> :<message>
				Channel channel = _server->getChannel(input.params[0]);
				if (channel->getMembersCount() <= 2)
				{
					//promuovo il messaggio a private message
					User receiver = channel->getMember(input.params[1]);
					PrivateMessage *msg = new PrivateMessage(input.params[2], *this, receiver);
					sendMessage(receiver, msg);
					break;
				}
				Message *msg = new Message(input.params[1], *this, *channel);
				sendMessage(channel, msg);
			}
			else
			{
				//private msg PRIVMSG <nickname> :<message>
				User receiver = _server->getClient(input.params[0]);
				PrivateMessage *msg = new PrivateMessage(input.params[1], *this, receiver);	
				sendMessage(receiver, msg);
			}
			break;
		//JOIN <channel1,channel2,channel3> <key1,key2,key3>
		case JOIN:
			client.checkConnection();
			vector<string> channels = split(input.params[0], ','); //se non e' in cpp98 mettiamolo in utils
			vector<string> keys = split(input.params[1], ',');
			
			for (size_t i = 0; i < channels.size(); i++)
			{
				Channel *channel = _server->getChannel(channels[i]);
				if (i < keys.size())
					joinChannel(*channel, keys[i]);
				else
					joinChannel(*channel);
			};
			break;
		case MODE:
			client.checkConnection();
			//TODO
			break;
		//PASS <connectionpassword>
		case PASS: //viene usata dagli utenti per autenticarsi
			hash<string>	hasher;
			
			if (_is_connected)
				throw AlreadyConnectedException();
			if (hasher(input.params[0]) != _server->getPwdHash())
				throw InvalidPasswordException();
			_is_connected = true;
			client.authenticate(); //una volta connesso, l'utente deve autenticarsi
			break;
		//NICK <nickname>
		case NICK:
			client.checkConnection();
			setNickname(input.params[0]);
			break;
		//USER <username>
		case USER:
			client.checkConnection();
			setUsername(input.params[0]);
			break;
		//QUIT
		case QUIT:
			//TODO
			break;
		default:
			throw UnkonwnCommandException();
	}
}

//input: ":<prefix> <command> <params> <crlf>"
t_input	EventHandler::parseInput(string &raw_input)
{
	t_input	input;
	string	command;
	static const unordered_map<string, t_cmd> commands = {
		{"PRIVMSG", PRIVMSG},
		{"JOIN", JOIN},
		{"MODE", MODE},
		{"PASS", PASS},
		{"NICK", NICK},
		{"USER", USER},
		{"QUIT", QUIT}
	};

	input.prefix = NULL;
	if (raw_input[0] == ':')
		input.prefix = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il prefix
	raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il prefix
	command = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il comando come stringa

	unordered_map<string, t_cmd>::const_iterator it;

	it = commands.find(command);
	if (it == commands.end())
		throw UnkonwnCommandException();
	input.command = commands.at(command); //associo il comando all'enum
	
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
}