/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 16:42:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/EventHandler.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Exceptions.hpp"

#include <algorithm>

using std::string;
using std::map;
using std::vector;
using std::exception;
using std::cout;
using std::endl;

Server::Server(Logger &logger, const uint16_t port_no, const string &password) :
	_port(port_no),
	_pwd_hash(hash(password)),
	_clients(map<string, Client *>()),
	_channels(map<string, Channel *>()),
	_pollfds(vector<pollfd>()),
	_socket(socket_p(AF_INET, SOCK_STREAM, 0)),
	_handler(EventHandler(logger, *this)),
	_logger(logger)
{
	struct sockaddr_in	server_addr;
	pollfd				server_poll_fd;
	char				hostname[256];
	struct hostent		*host;
	struct in_addr		**addr_list;

	memset(&server_addr, 0, sizeof(server_addr));
	configureNonBlocking(_socket);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);
    gethostname_p(hostname, sizeof(hostname));
    host = gethostbyname_p(hostname);
    addr_list = (struct in_addr **)host->h_addr_list;
    if (addr_list[0] != NULL)
	{
        char *ip = inet_ntoa(*addr_list[0]);
        _logger.logEvent("Server started on " + string(ip) + ":" + ::to_string(_port));
    }
	server_poll_fd.fd = _socket;
	server_poll_fd.events = POLLIN;
	_pollfds.push_back(server_poll_fd);
	_pollfds[0].fd = _socket;
	_pollfds[0].events = POLLIN;
	_pollfds[0].revents = 0;
}

Server::Server(const Server &copy) :
	_port(copy._port),
	_pwd_hash(copy._pwd_hash),
	_clients(copy._clients),
	_channels(copy._channels),
	_pollfds(copy._pollfds),
	_socket(copy._socket),
	_handler(copy._handler),
	_logger(copy._logger) {}

Server::~Server(void)
{
	//TODO delete di tutto
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
					handleNewClient();
				else
					handleClient(&i);
			}
		}
	}
}

void	Server::stop(void)
{
	Server::~Server();
}

uint16_t	Server::getPort(void) const
{
	return _port;
}

const string	&Server::getPwdHash(void) const
{
	return _pwd_hash;
}

const map<string, Client *>	&Server::getClients(void) const
{
	return _clients;
}

void	Server::setClients(const map<string, Client *> &clients)
{
	_clients = clients;
}

Client *Server::getClient(const string &nickname) const
{
	map<string, Client *>::const_iterator it = _clients.find(nickname);

	if (it == _clients.end())
		throw ProtocolErrorException(ERR_NOSUCHNICK, nickname);
	return it->second;
}

void Server::addClient(Client *client)
{
	const string &key = client->getPk();
	
	if (_clients.find(key) != _clients.end())
		throw InternalErrorException("Server::addClient: Client already exists");
	_clients[key] = client;
}

void Server::removeClient(const Client &client)
{
	map<string, Client *>::iterator it = _clients.find(client.getPk());

	if (it == _clients.end())
		throw InternalErrorException("Server::removeClient: Client not found");
	_clients.erase(it);
}

const map<string, Channel *>	&Server::getChannels(void) const
{
	return _channels;
}

void	Server::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

Channel	*Server::getChannel(const string &name) const
{
	map<string, Channel *>::const_iterator it = _channels.find(name);

	if (it == _channels.end())
		throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name);
	return it->second;
}

void	Server::addChannel(Channel &channel)
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) != _channels.end())
		throw InternalErrorException("Server::addChannel: Channel already exists");
	_channels[channel_name] = &channel;
}

void	Server::removeChannel(const Channel &channel)
{
	_channels.erase(channel.getName());
}

const vector<pollfd>	&Server::getPollfds(void) const
{
	return _pollfds;
}

void	Server::setPollfds(const vector<pollfd> &pollfds)
{
	_pollfds = pollfds;
}

void	Server::addPollfd(const pollfd &fd)
{
	vector<pollfd>::iterator it = std::find(_pollfds.begin(), _pollfds.end(), fd);

	if (it != _pollfds.end())
		throw InternalErrorException("Server::addPollfd: Pollfd already exists");
	_pollfds.push_back(fd);
}

void	Server::removePollfd(const pollfd &fd)
{
	vector<pollfd>::iterator it = std::find(_pollfds.begin(), _pollfds.end(), fd);

	if (it == _pollfds.end())
		throw InternalErrorException("Server::removePollfd: Pollfd not found");
	_pollfds.erase(it);
}

int	Server::getSocket(void) const
{
	return _socket;
}

bool Server::isClientConnected(const string &nickname) const
{
	return _clients.find(nickname) != _clients.end();
}

void Server::disconnectClient(Client &client)
{
	int socket = client.getSocket();

	shutdown_p(socket, SHUT_RDWR);
	close_p(socket);
	removePollfd(client.getPollfd());
	removeClient(client);
	delete &client;
}

void Server::configureNonBlocking(const int socket) const
{
	int flags;

	flags = fcntl(socket, F_GETFL); //TODO aggiungere fcntl a SystemCalls (probabilmente variadic function)
	if (flags == -1)
		throw SystemErrorException(strerror(errno));
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	if (flags == -1)
		throw SystemErrorException(strerror(errno));
}

void	Server::handleNewClient(void)
{
	Client				*client;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int 				client_socket;

	client_addr_len = sizeof(client_addr);
	client_socket = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	configureNonBlocking(client_socket);

	const string client_ip_addr = string(inet_ntoa(client_addr.sin_addr));
	const uint16_t client_port = ntohs(client_addr.sin_port);

	pollfd client_poll_fd;
	client_poll_fd.fd = client_socket;
	client_poll_fd.events = POLLIN;
	client_poll_fd.revents = 0;
	addPollfd(client_poll_fd);

	client = new Client(_logger, this, client_socket, client_poll_fd, client_ip_addr, client_port);
	addClient(client);

	_logger.logEvent("Incoming connection from " + client_ip_addr);
}

void Server::handleClient(size_t *i)
{
	Client	*client = NULL;

	for (map<string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getSocket() == _pollfds[*i].fd)
		{
			client = it->second;
			break;
		}
	}

	if (!client)
		throw InternalErrorException("Server::handleClient: Client not found");

	try
	{
		char	buffer[BUFFER_SIZE] = {0};
		int		bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);

		if (bytes_read > 0)
		{
			_handler.setClient(*client);
			_handler.processInput(buffer); // se tutto va bene esegue il comando
		}
		else
		{
			removeClient(*client);
			(*i)--;
			if (bytes_read < 0) // Error
				throw SystemErrorException(strerror(errno));
		}
	}
	catch (const ProtocolErrorException &e)
	{			
		client->receiveMessage(e.getContent());
		_logger.logError(&e);
	}
}

bool operator==(const pollfd &lhs, const pollfd &rhs)
{
    return lhs.fd == rhs.fd && lhs.events == rhs.events && lhs.revents == rhs.revents;
}