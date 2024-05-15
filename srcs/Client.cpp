/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/15 15:43:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

# include "headers/User.hpp"
# include "headers/Server.hpp"
# include "headers/EventHandler.hpp"

Client::Client(void) :
	User(),
	_is_connected(false),
	_ip_addr(0),
	_socket(0),
	_server() {}

Client::Client(const int socket) :
	User(),
	_is_connected(false),
	_ip_addr(0),
	_socket(socket),
	_server() {}

Client::Client(const Client &copy) :
	User(copy),
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
		_is_connected = rhs._is_connected;
		_ip_addr = rhs._ip_addr;
		_socket = rhs._socket;
		_server = rhs._server;
	}
	return *this;
}

bool	Client::getIsConnected(void) const
{
	return _is_connected;
}

uint16_t	Client::getPort(void) const
{
	return _port;
}

string	Client::getIpAddr(void) const
{
	return _ip_addr;
}

int		Client::getSocket(void) const
{
	return _socket;
}

Server	*Client::getServer(void) const
{
	return _server;
}

void	Client::setIsConnected(const bool is_connected)
{
	_is_connected = is_connected;
}

void	Client::setPort(const uint16_t port)
{
	_port = port;
}

void	Client::setIpAddr(const string ip_addr)
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
	char					buffer[1024];

    // Prompt user for password
    send(_socket, "Password: ", 10, 0);
    recv(_socket, buffer, sizeof(buffer), 0);
	_pwd_hash = Hasher::hash(string(buffer));

    // Check credentials
    if (_server->getUserPassword(_username) != _pwd_hash)
	{
		send(_socket, "Invalid credentials\n", 20, 0);
		_is_authenticated = false;
	}
	else
		_is_authenticated = true;
}

const char *Client::NotConnectedException::what(void) const throw()
{
	return "User is not connected";
}

const char *Client::AlreadyConnectedException::what(void) const throw()
{
	return "User is already connected";
}

const char *Client::InvalidPasswordException::what(void) const throw()
{
	return "Invalid password";
}

