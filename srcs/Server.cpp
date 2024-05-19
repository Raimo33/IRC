/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 17:17:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"
#include "headers/SystemCalls.hpp"
#include "headers/Client.hpp"
#include "headers/SystemCalls.hpp"
#include "headers/EventHandler.hpp"

Server::Server(const uint16_t port_no, const string &password) :
	_port(port_no),
	_pwd_hash(MD5(password).hexdigest()),
	_socket(socket_p(AF_INET, SOCK_STREAM, 0)),
	_event_handler(EventHandler())
{
	struct sockaddr_in server_addr;
	pollfd server_poll_fd;

	memset(&server_addr, 0, sizeof(server_addr));
	configureNonBlocking(_socket);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);
	bind_p(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen_p(_socket, 5);
	cout << "Server waiting for connections on port: " << _port << endl;
	server_poll_fd.fd = _socket;
	server_poll_fd.events = POLLIN;
	_pollfds.push_back(server_poll_fd);
}

Server::Server(const Server &copy) :
	_port(copy._port),
	_pwd_hash(copy._pwd_hash),
	_clients(copy._clients),
	_users(copy._users),
	_channels(copy._channels),
	_pollfds(copy._pollfds),
	_socket(copy._socket),
	_event_handler(copy._event_handler) {}

Server::~Server(void) {}

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
				{
					Client				*client;
					struct sockaddr_in	client_addr;
					socklen_t			client_addr_len;
					int 				client_socket;
					

					// TCP connection
					client_addr_len = sizeof(client_addr);
					client_socket = accept_p(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
					configureNonBlocking(client_socket);

					const string client_ip_addr = string(inet_ntoa(client_addr.sin_addr));
					uint16_t client_port = ntohs(client_addr.sin_port);

					client = new Client(this, client_socket, client_ip_addr, client_port);
					addClient(client);
					cout << "TCP Connection request from " << client->getIpAddr() << endl;
					handshake(client_socket); // SSL handshake
					client->setIsConnected(true);

					pollfd client_poll_fd;

					client_poll_fd.fd = client_socket;
					client_poll_fd.events = POLLIN;
					addPollfd(client_poll_fd);
					cout << "Connection established with " << client->getIpAddr() << endl;
				}
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

uint16_t	Server::getPort(void) const
{
	return _port;
}

const string	&Server::getPwdHash(void) const
{
	return _pwd_hash;
}

const map<int, Client *>	&Server::getClients(void) const
{
	return _clients;
}

void	Server::setClients(const map<int, Client *> &clients)
{
	_clients = clients;
}

const Client &Server::getClient(const string &username) const
{
	for (map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getUsername() == username)
			return *(it->second);
	}
	throw ClientNotFoundException();
}

const Client &Server::getClient(const int socket) const
{
	if (_clients.find(socket) == _clients.end())
		throw ClientNotFoundException();
	return *(_clients.at(socket));
}

void Server::addClient(Client *client)
{
	if (find(_clients.begin(), _clients.end(), client) != _clients.end())
		throw ClientAlreadyExistsException();
	_clients[client->getSocket()] = client;
}

void Server::removeClient(const Client &client)
{
	if (_clients.find(client.getSocket()) == _clients.end())
		throw ClientNotFoundException();
	_clients.erase(client.getSocket());
}

const map<string, User *>	&Server::getUsers(void) const
{
	return _users;
}

void	Server::setUsers(const map<string, User *> &users)
{
	_users = users;
}

const User &Server::getUser(const string &username) const
{
	if (_users.find(username) == _users.end())
		throw UserNotFoundException();
	return *(_users.at(username));
}

void Server::addUser(User *user)
{
	_users[user->getUsername()] = user;
}

void Server::removeUser(const User &user)
{
	_users.erase(user.getUsername());
}

const map<string, Channel *>	&Server::getChannels(void) const
{
	return _channels;
}

void	Server::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

const Channel	&Server::getChannel(const string &name) const
{
	if (_channels.find(name) == _channels.end())
		throw ChannelNotFoundException();
	return *(_channels.at(name));
}

void	Server::addChannel(Channel *channel)
{
	//TODO mettere un limite di canali per server
	if (_channels.find(channel->getName()) != _channels.end())
		throw ChannelAlreadyExistsException();
	_channels[channel->getName()] = channel;
}

void	Server::removeChannel(const Channel &channel)
{
	_channels.erase(channel.getName());
}

const vector<pollfd>	&Server::getPollfds(void)
{
	return _pollfds;
}

void	Server::setPollfds(const vector<pollfd> &pollfds)
{
	_pollfds = pollfds;
}

void	Server::addPollfd(const pollfd pollfd)
{
	_pollfds.push_back(pollfd);
}

void	Server::removePollfd(const pollfd pollfd)
{
	remove(_pollfds.begin(), _pollfds.end(), pollfd);
}

void Server::removePollfd(const int socket)
{
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == socket)
		{
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
	}
}

int	Server::getSocket(void) const
{
	return _socket;
}

const string	&Server::getUserPassword(const string &username) const
{
	for (map<string, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first == username)
			return it->second->getPwdHash();
	}
	throw UserNotFoundException();
}

void Server::handleClient(Client *client, size_t *i)
{
	char buffer[BUFFER_SIZE];

	int bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
	if (bytes_read > 0)
	{
		buffer[bytes_read - 1] = '\0';
		_event_handler.setClient(client);
		_event_handler.processInput(buffer); // se tutto va bene esegue il comando
		_event_handler.setClient(NULL);
	}
	else if (bytes_read <= 0)
	{
		// Connection closed
		removeClient(*client);
		(*i)--;
		// Error
		if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw SystemErrorException(strerror(errno));
	}
}

void Server::disconnectClient(Client *client)
{
	int socket = client->getSocket();

	shutdown_p(socket, SHUT_RDWR);
	close_p(socket);
	removePollfd(socket);
	removeClient(*client);
	delete client;
}

void	Server::handshake(const int client_socket) const
{
	//TODO: SSL handshake	
	(void)client_socket;
}

void Server::configureNonBlocking(const int socket) const
{
	int flags;

	flags = fcntl(socket, F_GETFL);
	if (flags == -1)
		throw runtime_error(strerror(errno));
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	if (flags == -1)
		throw runtime_error(strerror(errno));
}

const char *Server::ChannelAlreadyExistsException::what() const throw()
{
	return "Channel already exists";
}

const char *Server::ChannelNotFoundException::what() const throw()
{
	return "Channel not found";
}

const char *Server::UserAlreadyExistsException::what() const throw()
{
	return "User already exists";
}

const char *Server::UserNotFoundException::what() const throw()
{
	return "User not found";
}

const char *Server::InvalidPasswordException::what() const throw()
{
	return "Invalid password";
}

const char *Server::ClientNotFoundException::what() const throw()
{
	return "Client not found";
}

const char *Server::ClientAlreadyExistsException::what() const throw()
{
	return "Client already exists";
}

const char *Server::HandshakeFailedException::what() const throw()
{
	return "Handshake failed";
}


