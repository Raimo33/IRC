/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:21:17 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 18:35:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/EventHandler.hpp"
#include "irc/Server.hpp"
#include "irc/Channel.hpp"
#include "irc/ChannelOperator.hpp"
#include "irc/Hasher.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Exceptions.hpp"

using std::string;
using std::map;
using std::vector;

using namespace irc;

static void checkConnection(const Client *client);
static void checkAuthentication(const Client *client);

namespace irc
{
	EventHandler::EventHandler(void) :
		_server(NULL),
		_client(NULL),
		_commands(initCommands()),
		_handlers(initHandlers()) {}

	EventHandler::EventHandler(Server &server) :
		_server(&server),
		_client(NULL),
		_commands(initCommands()),
		_handlers(initHandlers()) {}

	EventHandler::EventHandler(const EventHandler &copy) :
		_server(copy._server),
		_client(copy._client),
		_commands(copy._commands),
		_handlers(copy._handlers) {}
		
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

	const Server	&EventHandler::getServer(void) const
	{
		return *_server;
	}

	void	EventHandler::setServer(Server &server)
	{
		_server = &server;
	}

	//JOIN #channel1,#channel2,#channel3 key1,key2,key3
	void	EventHandler::processInput(string raw_input)
	{
		s_commandContent input = parseInput(raw_input);

		if (input.cmd < 0 || input.cmd >= N_COMMANDS)
			throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, raw_input);	

		(this->*(_handlers[input.cmd]))(input.params);
	}

	const struct s_replyContent	EventHandler::buildReplyContent(const uint16_t code, const string *params, const string &custom_msg)
	{
		struct s_replyContent	content;

		content.prefix = string(SERVER_NAME);
		content.code = code;
		if (params)
		{
			size_t n_params;

			for (n_params = 0; params[n_params].empty() == false; n_params++);
			content.params = vector<string>(params, params + n_params);
		}
		if (custom_msg.empty())
		{
			map<uint16_t, string>::const_iterator it = reply_codes.find(code);
			if (it == reply_codes.end())
				throw InternalErrorException("EventHandler::buildReplyContent: Unknown reply code");
			content.text = it->second;
		}
		else
			content.text = custom_msg;
		return content;
	}

	const struct s_replyContent	EventHandler::buildReplyContent(const uint16_t code, const string param, const string &custom_msg)
	{
		const string params[] = { param };

		return buildReplyContent(code, params, custom_msg);
	}

	const struct s_commandContent	EventHandler::buildCommandContent(const string &prefix, const e_cmd_type cmd, const string *params, const string &custom_msg)
	{
		struct s_commandContent	content;

		content.prefix = prefix;
		content.cmd = static_cast<e_cmd_type>(cmd);
		if (params)
		{
			size_t n_params;

			for (n_params = 0; params[n_params].empty() == false; n_params++);
			content.params = vector<string>(params, params + n_params);
		}
		content.text = custom_msg;
		return content;
	}

	const struct s_commandContent	EventHandler::buildCommandContent(const string &prefix, const e_cmd_type cmd, const string param, const string &custom_msg)
	{
		const string params[] = { param };

		return buildCommandContent(prefix, cmd, params, custom_msg);
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
			getRawReplyMessage(receiver, reply, &first_part, &second_part);
		else
			getRawCommandMessage(command, &first_part, &second_part);
		block_size = MAX_MSG_LENGTH - first_part.size() - 2; //2 per \r\n
		do {
			send_length = min(block_size, second_part.size());
			to_send = first_part + second_part.substr(0, send_length) + "\r\n";
			second_part = second_part.substr(send_length); // Update second_part correctly
			send_p(receiver.getSocket(), to_send.c_str(), to_send.length(), 0);
		} while (second_part.empty() == false);
	}

	void	EventHandler::getRawReplyMessage(const Client &receiver, const struct s_replyContent *reply, string *first_part, string *second_part)
	{
		if (reply->prefix.empty() == false)
			*first_part += ":" + reply->prefix + " ";
		*first_part += irc::to_string(reply->code);
		*first_part += " " + receiver.getNickname() + " ";
		*first_part += join(reply->params, " ");
		if (reply->text.empty())
			*second_part += " :" + reply_codes.at(reply->code);
		else
			*second_part += " :" + reply->text;
	}

	void	EventHandler::getRawCommandMessage(const struct s_commandContent *command, string *first_part, string *second_part)
	{
		if (command->prefix.empty() == false)
			*first_part += ":" + command->prefix + " ";
		*first_part += _command_strings.at(command->cmd);
		*first_part += " ";
		*first_part += join(command->params, " ");
		*second_part += " :" + command->text;
	}

	const std::map<std::string, e_cmd_type>	EventHandler::initCommands(void)
	{
		static map<string, e_cmd_type>	commands;

		if (commands.empty() == false) //se il map e' gia' stato inizializzato
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

	//input: ":<prefix> <command> <params> <crlf>"
	s_commandContent	EventHandler::parseInput(string &raw_input) const
	{
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
		if (space_pos == string::npos)
			command = raw_input;
		else
			command = raw_input.substr(0, space_pos); //forse -1
		if (_commands.find(command) == _commands.end()) //se il comando non esiste
			throw ProtocolErrorException(ERR_UNKNOWNCOMMAND, command);
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

	void EventHandler::handlePass(const vector<string> &args)
	{
		if (_client->getIsConnected())
			throw ProtocolErrorException(ERR_ALREADYREGISTRED);

		if (args.size() < 1)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "PASS", "usage: PASS <password>");

		if (Hasher::hash(args[0]) != _server->getPwdHash())
			throw ProtocolErrorException(ERR_PASSWDMISMATCH);
		_client->setIsConnected(true);
	}

	void EventHandler::handleNick(const vector<string> &args)
	{
		checkConnection(_client);
		if (_client->getIsAuthenticated() || _client->getNickname().empty() == false)
			throw ProtocolErrorException(ERR_ALREADYREGISTRED);
		if (args.size() < 1)
			throw ProtocolErrorException(ERR_NONICKNAMEGIVEN);
		checkNicknameValidity(args[0]);
		_client->setNickname(args[0]);
		if (_client->getUsername().empty() == false)
			_client->setAuthenticated(true);
	}

	void EventHandler::handleUser(const vector<string> &args)
	{
		checkConnection(_client);
		if (_client->getIsAuthenticated() || _client->getUsername().empty() == false)
			throw ProtocolErrorException(ERR_ALREADYREGISTRED);
		if (args.size() < 1)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "USER", "usage: USER <username> <hostname> <servername> <realname>");
		_client->setUsername(args[0]);
		(void)args[1]; //args[1] = hostname
		(void)args[2]; //args[2] = servername
		_client->setRealname(args[3]);
		if (_client->getNickname().empty() == false)
			_client->setAuthenticated(true);
	}

	//JOIN <channel1,channel2,channel3> <key1,key2,key3>
	void EventHandler::handleJoin(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);
		if (args.size() < 1)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "JOIN", "usage: JOIN <channel> <key>");

		vector<string> channels = split(args[0], ',');
		vector<string> keys = split(args[1], ',');

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

	void EventHandler::handlePart(const vector<string> &args)
	{
		(void)args;
		//TODO implement
	}

	void EventHandler::handlePrivmsg(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);

		if (args.size() < 1)
			throw ProtocolErrorException(ERR_NORECIPIENT, "PRIVMSG", "no recipient given (PRIVMSG)");
		if (args.size() < 2)
			throw ProtocolErrorException(ERR_NOTEXTTOSEND);

		const struct s_commandContent msg = EventHandler::buildCommandContent(_client->getNickname(), PRIVMSG, NULL, args[1]);
		if (is_channel_prefix(args[0][0])) //se il primo carattere e' #, &, + o !
		{
			//channel msg PRIVMSG <channel> :<message>
			Channel channel = _server->getChannel(args[0]);
			_client->sendMessage(channel, msg);
		}
		else
		{
			//private msg PRIVMSG <nickname> :<message>
			Client receiver = _server->getClient(args[0]);
			_client->sendMessage(receiver, msg);
		}
	}


	void EventHandler::handleQuit(const vector<string> &args)
	{
		(void)args;
		_server->removeClient(*_client);
	}

	void EventHandler::handleKick(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);

		if (args.size() < 2)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "KICK", "usage: KICK <channel> <nickname> <reason>");

		Channel			channel = _server->getChannel(args[0]);
		Client			target = _server->getClient(args[1]);
		ChannelOperator	op(*_client);

		if (args.size() < 3)
			op.kick(target, channel);
		else
			op.kick(target, channel, args[2]);
	}

	void EventHandler::handleInvite(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);

		if (args.size() < 2)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "INVITE", "usage: INVITE <nickname> <channel>");

		Client			target = _server->getClient(args[0]);
		Channel			channel = _server->getChannel(args[1]);
		ChannelOperator	op(*_client);

		op.invite(target, channel);
	}

	void EventHandler::handleTopic(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);

		const size_t	n_args = args.size();

		if (n_args < 1)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "TOPIC", "usage: TOPIC <channel> [<topic>]");
		Channel channel = _client->getChannel(args[0]);
		if (n_args == 1)
		{
			const string params[] = { _client->getNickname(), channel.getName() };
			const struct s_replyContent topic = EventHandler::buildReplyContent(RPL_TOPIC, params, channel.getTopic()); // TODO valutare se mettere qualcosa quando ik topic [ vuoto ]
			EventHandler::sendBufferedContent(*_client, &topic);
		}
		else
		{
			if (channel.getMode(MODE_T))
			{
				ChannelOperator op(*_client);
				op.topicSet(channel, args[1]);
			}
			else
				channel.setTopic(args[1], *_client);
		}
	}

	void EventHandler::handleMode(const vector<string> &args)
	{
		checkConnection(_client);
		checkAuthentication(_client);

		if (args.size() < 2)
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, "MODE", "usage: MODE <channel> <mode>");
		//TODO implementare
		//(RPL_CHANNELMODEIS)
		//(RPL_TOPIC_WHO_TIME)
		//(RPL_TOPIC)
		//(ERR_UNKNOWNMODE)
		(void)args;
	}

	void	EventHandler::checkNicknameValidity(const string &nickname) const
	{
		if (is_valid_nickname(nickname) == false)
			throw ProtocolErrorException(ERR_ERRONEOUSNICKNAME, nickname, "allowed characters: A-Z, a-z, 0-9, -, [, ], \\, `, ^, {, }\nmax nickname lenght: " + to_string(MAX_NICKNAME_LEN));
		if (_server->isClientConnected(nickname))
			throw ProtocolErrorException(ERR_NICKNAMEINUSE, nickname);
	}

	const std::map<uint16_t, std::string> EventHandler::_command_strings = EventHandler::initCommandStrings();
}

static void checkConnection(const Client *client)
{
	if (client->getIsConnected() == false)
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not connected, use PASS <password> first");
}

static void checkAuthentication(const Client *client)
{
	if (client->getIsAuthenticated() == false)
		throw ProtocolErrorException(ERR_NOTREGISTERED, "you are not registered, use NICK <nickname> and USER <username> first");
}
