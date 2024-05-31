/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 17:18:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Messages.hpp"
#include "irc/Exceptions.hpp"

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
	char hostname[256];
	struct hostent *host;
	struct in_addr **addr_list;

	memset(&server_addr, 0, sizeof(server_addr));
	configureNonBlocking(_socket);
	configureNonBlocking(_epoll_fd);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = _socket;
	epoll_ctl_p(_epoll_fd, EPOLL_CTL_ADD, _socket, &event);

	gethostname_p(hostname, sizeof(hostname));
	host = gethostbyname_p(hostname);
	addr_list = (struct in_addr **)host->h_addr_list;
	if (addr_list[0] != NULL)
	{
		char *ip = inet_ntoa(*addr_list[0]);
		_logger.logEvent("Server started on " + std::string(ip) + ":" + ::to_string(_port));
	}
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
		disconnectClient(*it->second);
	for (map<string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete it->second;
	close_p(_socket);
	close_p(_epoll_fd);
	std::cout << "Goodbye, hope you have a great day without me :( " << std::endl;
}

void Server::run(void)
{
	const int MAX_EVENTS = 10;
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
					_logger.logEvent("Client disconnected: " + client->getIpAddr());
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
	throw ProtocolErrorException(ERR_NOSUCHNICK, nickname.c_str(), default_replies.at(ERR_NOSUCHNICK), NULL);
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
		throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name.c_str(), default_replies.at(ERR_NOSUCHCHANNEL), NULL);
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
	int socket = client.getSocket();

	shutdown_p(socket, SHUT_RDWR);
	close_p(socket);
	removeClient(client);
	_logger.logEvent("Client disconnected: " + client.getIpAddr());
	delete &client;
}

void Server::configureNonBlocking(const int socket) const
{
	int flags;

	flags = fcntl_p(socket, F_GETFL);
	fcntl_p(socket, F_SETFL, flags | O_NONBLOCK);
}

void Server::handleNewClient(void)
{
	Client				*client;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len = sizeof(client_addr);
	const int			client_socket = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);

	configureNonBlocking(client_socket);

	const std::string client_ip_addr = std::string(inet_ntoa(client_addr.sin_addr));
	const uint16_t client_port = ntohs(client_addr.sin_port);

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN | EPOLLHUP | EPOLLERR;
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
		string	raw_input;
		char	buffer[BUFFER_SIZE];

		raw_input.reserve(BUFFER_SIZE);
		while (true)
		{
			memset(buffer, 0, sizeof(buffer));
			int bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
			if (bytes_read <= 0)
			{
				disconnectClient(*client);
				if (bytes_read < 0)
					throw SystemErrorException(strerror(errno));
				break;
			}
			raw_input += buffer;
			if (raw_input.find("\r\n") != string::npos)
				break;
		}
		_handler.setClient(*client);
		_handler.processInput(raw_input);
	}
	catch (const ProtocolErrorException &e) //TODO vlautare se catchare le SystemErrorException qui, dato che il server non deve mai crashare
	{
		struct s_message msg = e.getContent();
		msg.params.insert(msg.params.begin(), client->getNickname());
		client->receiveMessage(msg);
		_logger.logError(&e);
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
