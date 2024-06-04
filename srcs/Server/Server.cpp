/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 17:06:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "EventHandler.hpp"
#include "server_exceptions.hpp"
#include "system_calls.hpp"

#include <algorithm>
#include <arpa/inet.h>

using std::cout;
using std::endl;
using std::exception;
using std::map;
using std::string;
using std::vector;

Server::Server(Logger &logger, const uint16_t port_no, const string &password) :
    _port(port_no),
    _pwd_hash(hash(password)),
    _epoll_fd(epoll_create1_p(0)),
    _socket(socket_p(AF_INET, SOCK_STREAM, 0)),
    _handler(EventHandler(logger, *this)),
    _logger(logger)
{
	struct sockaddr_in server_addr;
	socklen_t          addr_len = sizeof(server_addr);
	char               ipstr[INET_ADDRSTRLEN];

	memset(&server_addr, 0, sizeof(server_addr));
	configure_non_blocking(_socket);
	server_addr.sin_family      = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port        = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events  = EPOLLIN;
	event.data.fd = _socket;
	epoll_ctl_p(_epoll_fd, EPOLL_CTL_ADD, _socket, &event);

	getsockname_p(_socket, (struct sockaddr *)&server_addr, &addr_len);
	inet_ntop(AF_INET, &server_addr.sin_addr, ipstr, sizeof(ipstr));
	ostringstream oss;
	oss << "Server listening on " << ipstr << ":" << _port;
	_logger.logEvent(oss.str());
}

Server::Server(const Server &copy) :
    _port(copy._port),
    _pwd_hash(copy._pwd_hash),
    _clients(copy._clients),
    _channels(copy._channels),
    _epoll_fd(copy._epoll_fd),
    _socket(copy._socket),
    _handler(copy._handler),
    _logger(copy._logger) {}

Server::~Server(void)
{
	for (map<string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		delete it->second;
	close_p(_socket);
	close_p(_epoll_fd);
}

void Server::run(void)
{
	const int          MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		int n = epoll_wait_p(_epoll_fd, events, MAX_EVENTS, -1);

		for (int i = 0; i < n; i++)
		{
			if (events[i].events & EPOLLIN)
			{
				if (events[i].data.fd == _socket)
					handleNewClient();
				else
					handleClient(events[i].data.fd);
			}
			else if (events[i].events & (EPOLLHUP | EPOLLERR))
			{
				Client *client = getClient(events[i].data.fd);
				if (client)
					disconnectClient(*client);
				else
					_logger.logEvent("Client disconnected");
			}
		}
	}
}

void Server::stop(void)
{
	Server::~Server();
}

uint16_t Server::getPort(void) const
{
	return _port;
}

const string &Server::getPwdHash(void) const
{
	return _pwd_hash;
}

const map<string, Client *> &Server::getClients(void) const
{
	return _clients;
}

void Server::setClients(const map<string, Client *> &clients)
{
	_clients = clients;
}

Client &Server::getClient(const string &nickname) const
{
	for (map<string, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return *it->second;
	}
	throw ActionFailedException(ERR_NOSUCHNICK, nickname.c_str(), g_default_replies_map.at(ERR_NOSUCHNICK), NULL);
}

void Server::addClient(Client &client)
{
	const string &key = client.getPk();

	if (_clients.find(key) != _clients.end())
		throw InternalErrorException("Server::addClient: Client already exists");
	_clients[key] = &client;
}

void Server::removeClient(const Client &client)
{
	map<string, Client *>::iterator it = _clients.find(client.getPk());

	if (it == _clients.end())
		throw InternalErrorException("Server::removeClient: Client not found");
	_clients.erase(it);
}

const map<string, Channel *> &Server::getChannels(void) const
{
	return _channels;
}

void Server::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

Channel &Server::getChannel(const string &name) const
{
	map<string, Channel *>::const_iterator it = _channels.find(name);

	if (it == _channels.end())
		throw ActionFailedException(ERR_NOSUCHCHANNEL, name.c_str(), g_default_replies_map.at(ERR_NOSUCHCHANNEL), NULL);
	return *it->second;
}

void Server::addChannel(Channel &channel)
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) != _channels.end())
		throw InternalErrorException("Server::addChannel: Channel already exists");
	_channels[channel_name] = &channel;
}

void Server::removeChannel(const Channel &channel)
{
	_channels.erase(channel.getName());
}

int Server::getSocket(void) const
{
	return _socket;
}

bool Server::isClientConnected(const string &nickname) const
{
	for (map<string, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::disconnectClient(Client &client)
{
	int          socket  = client.getSocket();
	const string ip_addr = client.getIpAddr();

	removeClient(client);
	delete &client;
	shutdown_p(socket, SHUT_RDWR);
	close_p(socket);
	_logger.logEvent("Client disconnected: " + ip_addr);
}

void Server::handleNewClient(void)
{
	Client            *client;
	struct sockaddr_in client_addr;
	socklen_t          client_addr_len = sizeof(client_addr);
	const int          client_socket   = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);

	configure_non_blocking(client_socket);

	const std::string client_ip_addr = std::string(inet_ntoa(client_addr.sin_addr));
	const uint16_t    client_port    = ntohs(client_addr.sin_port);

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events  = EPOLLIN | EPOLLHUP | EPOLLERR;
	event.data.fd = client_socket;
	epoll_ctl_p(_epoll_fd, EPOLL_CTL_ADD, client_socket, &event);

	client = new Client(_logger, this, client_socket, client_ip_addr, client_port);
	addClient(*client);

	_logger.logEvent("Incoming connection from " + client_ip_addr);
}

void Server::handleClient(const int client_socket)
{
	Client *client = getClient(client_socket);

	if (!client)
		return;

	try
	{
		char buffer[BUFFER_SIZE] = { 0 };

		if (recv_p(client_socket, buffer, sizeof(buffer) - 1, 0) == 0)
			return;
		_handler.setClient(*client);
		_handler.processInput(buffer);
	}
	catch (ActionFailedException &e) // TODO vlautare se catchare le SystemErrorException qui, dato che il server non deve mai crashare
	{
		ReplyMessage &reply = e.getContent();
		if (client->getIsAuthenticated())
			reply.setParam(client->getNickname(), 0);
		else
			reply.setParam(SERVER_NAME, 0);
		client->receiveMessage(&reply);
		ostringstream oss;
		oss << "Action failed: " << e.what();
		_logger.logEvent(oss.str());
	}
}

Client *Server::getClient(const int client_socket) const
{
	for (map<string, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getSocket() == client_socket)
			return it->second;
	}
	return (NULL);
}
