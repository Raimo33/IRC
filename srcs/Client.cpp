/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 16:47:50 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

# include "headers/User.hpp"
# include "headers/Server.hpp"
# include "headers/EventHandler.hpp"

Client::Client(const Server *server, const int socket, const string &ip_addr, const uint16_t port) :
	User(),
	_is_connected(true),
	_port(port),
	_ip_addr(ip_addr),
	_socket(socket),
	_server(server) {}

Client::Client(const Client &copy) :
	User(copy),
	_is_connected(copy._is_connected),
	_port(copy._port),
	_ip_addr(copy._ip_addr),
	_socket(copy._socket),
	_server(copy._server) {}

Client::~Client() {}

bool	Client::getIsConnected(void) const
{
	return _is_connected;
}

void	Client::setIsConnected(const bool is_connected)
{
	if (_is_connected == is_connected)
		throw AlreadyConnectedException();
	_is_connected = is_connected;
}

uint16_t	Client::getPort(void) const
{
	return _port;
}

const string	Client::getIpAddr(void) const
{
	return _ip_addr;
}

int	Client::getSocket(void) const
{
	return _socket;
}

const Server	*Client::getServer(void) const
{
	return _server;
}

void	Client::authenticate(void)
{
	char	buffer[1024];
	// Prompt user for password
	send(_socket, "Password: ", 10, 0);
    recv(_socket, buffer, sizeof(buffer), 0);

	MD5		hasher(string(buffer));

	_pwd_hash = hasher.hexdigest();

	//TODO gestire il caso in cui l'utente non ha mai fatto il signup
    // Check credentials
	try
	{
		if (_server->getUserPassword(_username) != _pwd_hash)
		{
			send(_socket, "Invalid credentials\n", 20, 0);
			_is_authenticated = false;
			return ;
		}
	}
	catch(const Server::UserNotFoundException &e)
	{
		send(_socket, "User not found: signing up\n", 28, 0);
		send(_socket, "Password: ", 10, 0);
    	recv(_socket, buffer, sizeof(buffer), 0);
		_pwd_hash = Hasher::hash(string(buffer));
		send(_socket, "Repeat password: ", 17, 0);
		recv(_socket, buffer, sizeof(buffer), 0);
		if (_pwd_hash != Hasher::hash(string(buffer)))
		{
			send(_socket, "Passwords don't match\n", 22, 0);
			_is_authenticated = false;
			return ;
		}
	}
	_is_authenticated = true;
	_server->addUser(*this);
	_server->addCredentials(_username, _pwd_hash);
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

