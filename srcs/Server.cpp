/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/15 16:29:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"
#include "headers/wrappers.hpp"
#include "headers/Client.hpp"
#include "headers/wrappers.hpp"
#include "headers/EventHandler.hpp"

Server::Server(const uint16_t port_no, const string &password) :
	_port(port_no),
	_pwd_hash(Hasher::hash(password)),
	_clients(),
	_channels(),
	_pollfds(),
	_socket(0) {}

Server::Server(const Server &copy) :
	_port(copy._port),
	_pwd_hash(copy._pwd_hash),
	_clients(copy._clients),
	_channels(copy._channels),
	_pollfds(copy._pollfds),
	_socket(copy._socket) {}

Server::~Server() {}

Server	&Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		_pwd_hash = rhs._pwd_hash;
		_port = rhs._port;
		_clients = rhs._clients;
		_channels = rhs._channels;
		_pollfds = rhs._pollfds;
		_socket = rhs._socket;
	}
	return *this;
}

void	Server::setup(void)
{
	struct sockaddr_in	server_addr;
	pollfd				server_poll_fd;

	_socket = socket_p(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	configureNonBlocking(_socket);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);
	cout << "Server waiting for connections on port " << _port << endl;
    server_poll_fd.fd = _socket;
    server_poll_fd.events = POLLIN;
    _pollfds.push_back(server_poll_fd);
}

void	Server::run(void)
{
	while (true)
	{
		poll_p(&_pollfds[0], _pollfds.size(), -1);
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents & POLLIN)
			{
				if (_pollfds[i].fd == _socket)
					addClient();
				else
				{
					Client *client = NULL;
					for (size_t j = 0; j < _clients.size(); j++)
					{
						if (_clients[j]->getSocket() == _pollfds[i].fd)
						{
							client = _clients[j];
							break;
						}
					}
					if (client)
						handleClient(client, &i);
				}
			}
		}
	}
}

void	Server::handleClient(Client *client, size_t *i)
{
	char			buffer[BUFFER_SIZE];

	int	bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
	if (bytes_read > 0)
	{
		buffer[bytes_read - 1] = '\0';
		EventHandler handler(client, this);
		handler.processInput(buffer);  //se tutto va bene esegue il comando
	}
	else if (bytes_read <= 0)
	{
		// Connection closed
		removeClient(client);
		(*i)--;
		// Error
		if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw runtime_error(strerror(errno));
	}
}

void	Server::addClient(void)
{
	Client				*client;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					client_socket;

	//TCP connection
	client_addr_len = sizeof(client_addr);
	client_socket = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	configureNonBlocking(client_socket);

	client = new Client(client_socket);
	client->setServer(this);
	client->setIpAddr(string(inet_ntoa(client_addr.sin_addr)));
	client->setPort(ntohs(client_addr.sin_port));
	_clients.push_back(client);

	cout << "TCP Connection request from " << client->getIpAddr() << endl;
	handshake(client_socket); //SSL handshake

    pollfd client_poll_fd;
    client_poll_fd.fd = client_socket;
    client_poll_fd.events = POLLIN;
    _pollfds.push_back(client_poll_fd);

	cout << "Connection established with " << client->getIpAddr() << endl;
}

void	Server::removeClient(Client *client)
{
		shutdown_p(client->getSocket(), SHUT_RDWR);
		close_p(client->getSocket());
		_pollfds.erase(remove(_pollfds.begin(), _pollfds.end(), pollfd{client->getSocket(), POLLIN}), _pollfds.end());
		_clients.erase(remove(_clients.begin(), _clients.end(), *client), _clients.end());
		delete client;
}

void	Server::configureNonBlocking(const int socket) const
{
	int	flags;

	flags = fcntl(socket, F_GETFL);
	if (flags == -1)
		throw runtime_error(strerror(errno));
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	if (flags == -1)
		throw runtime_error(strerror(errno));
}

void	Server::handshake(const int client_socket) const
{
	//TODO: SSL handshake	
	(void)client_socket;
}

void	Server::addChannel(Channel &channel)
{
	_channels[channel.getName()] = &channel;
}

Channel	&Server::getChannel(const string &name) const
{
	return *(_channels.at(name));
}

size_t	Server::getPwdHash(void) const
{
	return _pwd_hash;
}

size_t	Server::getUserPassword(const string &username) const
{
	return _credentials.at(username);
}
