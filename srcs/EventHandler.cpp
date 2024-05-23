/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 19:10:09 by craimond         ###   ########.fr       */
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

namespace irc
{
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
		s_commandContent input = parseInput(raw_input);

		if (input.cmd < 0 || input.cmd >= N_COMMANDS)
			throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, raw_input.c_str());

		(this->*(_handlers[input.cmd]))(input.params);
	}

	void	EventHandler::sendBufferedContent(const Client &receiver, const struct s_contentBase *message)
	{
		string							first_part;
		string							second_part;
		uint16_t						block_size;
		uint16_t						send_length;
		string							to_send;
		const struct s_replyContent		*reply = dynamic_cast<const s_replyContent *>(message);
		const struct s_commandContent	*command = dynamic_cast<const s_commandContent *>(message);
		

		if (!reply && !command)
			throw InternalErrorException("EventHandler::sendBufferedContent: Invalid message type");
		if (command && (command->cmd < 0 || command->cmd >= N_COMMANDS))
			throw InternalErrorException("EventHandler::sendBufferedContent: Invalid command type");
		if (reply && reply_codes.find(reply->code) == reply_codes.end())
			throw InternalErrorException("EventHandler::sendBufferedContent: Invalid reply code");

		if (reply)
			buildRawReplyMessage(reply, &first_part, &second_part);
		else
			buildRawCommandMessage(command, &first_part, &second_part);
		block_size = MAX_MSG_LENGTH - first_part.size() - 2; //2 per \r\n
		do {
			send_length = min(block_size, second_part.size());
			to_send = first_part + second_part.substr(0, send_length) + "\r\n";
			second_part = second_part.substr(send_length); // Update second_part correctly
			send_p(receiver.getSocket(), to_send.c_str(), to_send.length(), 0);
		} while (!second_part.empty());
	}

	void	EventHandler::buildRawReplyMessage(const struct s_replyContent *reply, string *first_part, string *second_part)
	{
		if (!reply->prefix.empty())
			*first_part += ":" + reply->prefix + " ";
		*first_part += irc::to_string(reply->code);
		*first_part += " ";
		*first_part += join(reply->params, " ");
		if (reply->text.empty())
			*second_part += " :" + reply_codes.at(reply->code);
		else
			*second_part += " :" + reply->text;
	}

	void	EventHandler::buildRawCommandMessage(const struct s_commandContent *command, string *first_part, string *second_part)
	{
		if (!command->prefix.empty())
			*first_part += ":" + command->prefix + " ";
		*first_part += _command_strings.at(command->cmd);
		*first_part += " ";
		*first_part += join(command->params, " ");
		*second_part += " :" + command->text;
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

		commands["PASS"] = PASS;
		commands["NICK"] = NICK;
		commands["USER"] = USER;
		commands["JOIN"] = JOIN;
		commands["PRIVMSG"] = PRIVMSG;
		commands["QUIT"] = QUIT;

		commands["KICK"] = KICK;
		commands["INVITE"] = INVITE;
		commands["TOPIC"] = TOPIC;
		commands["MODE"] = MODE;

		return commands;
	}

	std::map<uint16_t, std::string> EventHandler::initCommandStrings(void)
	{
		static std::map<uint16_t, std::string>	command_strings;

		if (command_strings.empty())
		{
			command_strings[PRIVMSG] = "PRIVMSG";
			command_strings[JOIN] = "JOIN";
			command_strings[PASS] = "PASS";
			command_strings[NICK] = "NICK";
			command_strings[USER] = "USER";
			command_strings[QUIT] = "QUIT";

			command_strings[KICK] = "KICK";
			command_strings[INVITE] = "INVITE";
			command_strings[TOPIC] = "TOPIC";
			command_strings[MODE] = "MODE";
		}
		return command_strings;
	}

	//input: ":<prefix> <command> <params> <crlf>"
	s_commandContent	EventHandler::parseInput(string &raw_input) const
	{
		//TODO rifare
		s_commandContent	input;
		string				command;
		size_t				space_pos;

		raw_input = raw_input.substr(0, MAX_MSG_LENGTH); //limito la lunghezza dell'input (per evitare buffer overflow
		if (raw_input.size() >= 2 && has_crlf(raw_input))
			raw_input.resize(raw_input.size() - 2);
		if (raw_input[0] == ':')
		{
			space_pos = raw_input.find(' ');
			input.prefix = raw_input.substr(1, space_pos - 1); //prendo il prefix
			raw_input = raw_input.substr(space_pos + 1); //supero il prefix
		}
		space_pos = raw_input.find(' ');
		command = raw_input.substr(0, space_pos - 1); //prendo il comando come stringa
		if (_commands.find(command) == _commands.end()) //se il comando non esiste
			throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command.c_str());
		input.cmd = _commands.at(command); //associo il comando all'enum
		raw_input = raw_input.substr(space_pos + 1); //supero il comando

		string param;

		while ((space_pos = raw_input.find(' ')) != string::npos) //finchè ci sono parametri
		{
			if (raw_input[0] == ':') //se il parametro inizia con ':' allora tutto ciò che segue è il parametro (anche spazi, caratteri speciali)
			{
				input.text = raw_input.substr(1); //prendo tutto il resto apparte il ':'
				break;
			}
			param = raw_input.substr(0, space_pos); //prendo il parametro
			input.params.push_back(param); //aggiungo il parametro al vettore
			raw_input = raw_input.substr(space_pos + 1); //supero il parametro
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
				if (e.getContent().code == ERR_NOSUCHCHANNEL)
				{
					ChannelOperator op(*_client);
					Channel			*new_channel;

					if (i < keys.size())
					{
						new_channel = new Channel(channels[i], keys[i], op);	
						_server->addChannel(new_channel);
					}
					else
					{
						new_channel = new Channel(channels[i], op);
						_server->addChannel(new_channel);
					}
					_client->joinChannel(*new_channel);
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

		(void)params;	
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
			throw ProtocolErrorException(ERR_ERRONEOUSNICKNAME, nickname.c_str()); //TODO aggiungere la reason (allowed characters: A-Z, a-z, 0-9, -, [, ], \, `, ^, {, }, MAX_NICKNAME_LEN)
		if (_server->isClientConnected(nickname))
			throw ProtocolErrorException(ERR_NICKNAMEINUSE, nickname.c_str());
	}

	const std::map<uint16_t, std::string> EventHandler::_command_strings = EventHandler::initCommandStrings();
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
