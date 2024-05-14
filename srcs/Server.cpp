/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 11:49:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

Server::Server() : 
	_port(DEFAULT_PORT), //default port
	_clients(),
	_channels(),
	_pollfds()
	_socket(0),
{
	hash<string>	hasher;

	_pwd_hash = hasher(PASSWORD);
}

Server::Server(const uint16_t port_no) :
	_port(port_no),
	_clients(),
	_channels(),
	_pollfds(),
	_socket(0)
{
	hash<string>	hasher;

	_pwd_hash = hasher(PASSWORD);
}

Server::Server(const Server &copy) :
	_pwd_hash(copy._pwd_hash),
	_port(copy._port),
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

void	Server::run(void)
{
	struct sockaddr_in	server_addr;

	_socket = socket_p(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 3);
	cout << "Server waiting for connections on port " << _port << endl;
	while (true) //threading
		addClient();
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
	cout << "TCP Connection request from " << client->getIpAddr() << ":" << client->getPortNbr() << endl;

	cout << "SSL handhaking with " << client->getIpAddr() << ":" << client->getPortNbr() << endl;
	handshake(client_socket); //SSL handshake

	//Add client to the list
	client = new Client(client_socket);
	client->setServer(this);
	client->setIpAddr(inet_ntoa(client_addr.sin_addr));
	client->setPortNbr(ntohs(client_addr.sin_port));
	_clients.push_back(client);

	thread client_thread(&Client::run, client); //is_connected verra' settato qua dentro dopo il login
	client_thread.detach();

	cout << "Connection established with " << client->getIpAddr() << ":" << client->getPortNbr() << endl;
}

void	Server::handshake(int client_socket)
{
	//TODO: aggiungere SSL handshake
}

void	Server::addChannel(const Channel &channel)
{
	_channels[channel.getName()] = &channel;
}
