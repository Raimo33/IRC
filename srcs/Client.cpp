/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:00:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

# include "headers/User.hpp"
# include "headers/Server.hpp"
# include "headers/EventHandler.hpp"
# include "headers/SystemCalls.hpp"

Client::Client(Server *server, const int socket, const string &ip_addr, const uint16_t port) :
	User(),
	_is_connected(false),
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
	{
		if (is_connected)
			throw AlreadyConnectedException();
		else
			throw AlreadyDisconnectedException();
	}
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

Server	*Client::getServer(void) const
{
	return _server;
}

void	Client::authenticate(void)
{
	char	buffer[1024];
	MD5		hasher;
	uint32_t len;

	send_p(_socket, "Password: ", 10, 0);
    len = recv_p(_socket, buffer, sizeof(buffer), 0);
	hasher.update(buffer, len);
	_pwd_hash = hasher.hexdigest();
	try
	{
		const User user = _server->getUser(_username);

		if (user.getPwdHash() == _pwd_hash)
		{
			_is_authenticated = true;
			_server->addUser(this);
		}
		else
		{
			send_p(_socket, "Invalid credentials\n", 20, 0);
			_is_authenticated = false;
		}
	}
	catch(const Server::UserNotFoundException &e)
	{
		send_p(_socket, "User not found: signing up\n", 28, 0);
		send_p(_socket, "Password: ", 10, 0);
    	len = recv_p(_socket, buffer, sizeof(buffer), 0);
		hasher.update(buffer, len);
		_pwd_hash = hasher.hexdigest();
		send_p(_socket, "Repeat password: ", 17, 0);
		len = recv_p(_socket, buffer, sizeof(buffer), 0);
		hasher.update(buffer, len);
		if (_pwd_hash == hasher.hexdigest())
		{
			_is_authenticated = true;
			_server->addUser(this);
		}
		else
		{
			send_p(_socket, "Passwords don't match\n", 22, 0);
			_is_authenticated = false;
		}
	}
}

const char *Client::NotConnectedException::what(void) const throw()
{
	return "Client is not connected";
}

const char *Client::AlreadyConnectedException::what(void) const throw()
{
	return "Client is already connected";
}

const char *Client::AlreadyDisconnectedException::what(void) const throw()
{
	return "Client is already disconnected";
}

const char *Client::InvalidPasswordException::what(void) const throw()
{
	return "Invalid password";
}

