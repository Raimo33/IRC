/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 18:33:36 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

Client::Client() :
	User(),
	_host_name(""),
	_is_connected(false),
	_ip_addr(0),
	_socket(0),
	_server() {}

Client::Client(const int socket) :
	User(),
	_host_name(""),
	_is_connected(false),
	_ip_addr(0),
	_socket(socket),
	_server() {}

Client::Client(const Client &copy) :
	User(copy),
	_host_name(copy._host_name),
	_is_connected(copy._is_connected),
	_ip_addr(copy._ip_addr),
	_socket(copy._socket),
	_server(copy._server) {}

Client::~Client() {}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		User::operator=(rhs);
		_host_name = rhs._host_name;
		_is_connected = rhs._is_connected;
		_ip_addr = rhs._ip_addr;
		_socket = rhs._socket;
		_server = rhs._server;
	}
	return *this;
}

bool	Client::getIsConnected() const
{
	return _is_connected;
}

uint16_t	Client::getPortNo() const
{
	return _port_no;
}

uint32_t	Client::getIpAddr() const
{
	return _ip_addr;
}

int		Client::getSocket() const
{
	return _socket;
}

Server	*Client::getServer() const
{
	return _server;
}

void	Client::setIsConnected(const bool is_connected)
{
	_is_connected = is_connected;
}

void	Client::setPortNo(const uint16_t port_no)
{
	_port_no = port_no;
}

void	Client::setIpAddr(const uint32_t ip_addr)
{
	_ip_addr = ip_addr;
}

void	Client::setSocket(const int socket)
{
	_socket = socket;
}

void	Client::setServer(Server *server)
{
	_server = server;
}

void	Client::run(void)
{
	char	buffer[BUFFER_SIZE] = {0};
	t_input	input;

	while (_is_connected)
	{
		if (recv(_socket, buffer, BUFFER_SIZE - 1, 0) <= 0)
			break;
		buffer[BUFFER_SIZE - 1] = '\0';
		input = parseInput(buffer);
		processInput(input); //se tutto va bene esegue il cmando
		memset(buffer, 0, BUFFER_SIZE);
	}
	_is_connected = false;
	perror("recv");
}

void	Client::processInput(const t_input &input)
{
	switch (input.command)
	{
		case PRIVMSG: //usato sia per messaggi privati che per messaggi nei canali
			checkConnection();
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
			checkConnection();
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
			checkConnection();
			//TODO
			break;
		//PASS <connectionpassword>
		case PASS: //viene usata dagli utenti per autenticarsi
			hash<string>	hasher;
			
			if (_is_connected)
				UserAlreadyConnectedException();
			if (hasher(input.params[0]) != _server->getPwdHash())
				InvalidPasswordException();
			_is_connected = true;
			authenticate(_socket); //una volta connesso, l'utente deve autenticarsi
			break;
		//NICK <nickname>
		case NICK:
			checkConnection();
			setNickname(input.params[0]);
			break;
		//USER <username>
		case USER:
			checkConnection();
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
t_input	Client::parseInput(const string &raw_input)
{
	t_input	input;

	input.prefix = NULL;
	if (raw_input[0] == ':')
		input.prefix = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il prefix
	raw_input = raw_input.substr(raw_input.find(' ') + 1); //supero il prefix
	input.command = raw_input.substr(1, raw_input.find(' ') - 1); //prendo il comando
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
//output: {string prefix, enum command, {string param1, string param2, ...}}

void	Client::checkConnection(void) const
{
	if (!_is_connected)
		throw NotConnectedException();
	if (!_is_authenticated)
		throw User::NotAuthenticatedException();
}

void	Client::authenticate(void)
{
	char			buffer[1024];
	hash<string>	hasher;

    // Prompt user for username
    send(_socket, "Username: ", 10, 0);
    recv(_socket, buffer, sizeof(buffer), 0);
	buffer[strlen(buffer) - 1] = '\0';
	_username = string(buffer);

    // Prompt user for password
    send(_socket, "Password: ", 10, 0);
    recv(_socket, buffer, sizeof(buffer), 0);
	_pwd_hash = hasher(string(buffer));

    // Check credentials
    if (_server->getPwdHash(_username) != _pwd_hash)
	{
		send(_socket, "Invalid credentials\n", 20, 0);
		_is_authenticated = false;
	}
	else
		_is_authenticated = true;
}