/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 12:23:32 by craimond         ###   ########.fr       */
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
	char			buffer[BUFFER_SIZE] = {0};
	t_input			input;
	EventHandler	dispatcher;

	while (_is_connected)
	{
		if (recv(_socket, buffer, BUFFER_SIZE - 1, 0) <= 0)
			break;
		buffer[BUFFER_SIZE - 1] = '\0';
		dispatcher.processInput(buffer, *this); //se tutto va bene esegue il cmando
		memset(buffer, 0, BUFFER_SIZE);
	}
	_is_connected = false;
	perror("recv");
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

const char *Client::UnknownCommandException::what() const throw()
{
	return "Unknown command";
}

const char *Client::NotConnectedException::what() const throw()
{
	return "User is not connected";
}

const char *Client::AlreadyConnectedException::what() const throw()
{
	return "User is already connected";
}

const char *Client::InvalidPasswordException::what() const throw()
{
	return "Invalid password";
}

