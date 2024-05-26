/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 19:00:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Server.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/SystemCalls.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Hasher.hpp"
#include "irc/ReplyCodes.hpp"

#include "irc/Exceptions.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;

namespace irc
{
	Server::Server(const uint16_t port_no, const string &password) :
		_port(port_no),
		_pwd_hash(Hasher::hash(password)),
		_clients(map<string, Client *>()),
		_channels(map<string, Channel *>()),
		_pollfds(vector<pollfd>()),
		_socket(socket_p(AF_INET, SOCK_STREAM, 0)),
		_handler(EventHandler(*this))
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
		//TODO printare l'IP del server
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
		_handler(copy._handler) {}


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
					//new connection
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
						client->setIsConnected(true);

						pollfd client_poll_fd;

						client_poll_fd.fd = client_socket;
						client_poll_fd.events = POLLIN;
						addPollfd(client_poll_fd);
						cout << "Connection established with " << client->getIpAddr() << endl;
					}
					else
					{
						//client already exists
						Client *client = NULL;

						for (map<string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
						{
							if (it->second->getSocket() == _pollfds[i].fd)
							{
								client = it->second;
								break;
							}
						}
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

	const map<string, Client *>	&Server::getClients(void) const
	{
		return _clients;
	}

	void	Server::setClients(const map<string, Client *> &clients)
	{
		_clients = clients;
	}

	const Client &Server::getClient(const string &nickname) const
	{
		if (_clients.find(nickname) == _clients.end())
			throw ProtocolErrorException(ERR_NOSUCHNICK, nickname);
		return *(_clients.at(nickname));
	}

	void Server::addClient(Client *client)
	{
		if (_clients.find(client->getNickname()) != _clients.end())
			throw InternalErrorException("Server::addClient: Client already exists");
		_clients[client->getNickname()] = client;
	}

	void Server::removeClient(const Client &client)
	{
		if (_clients.find(client.getNickname()) == _clients.end())
			throw InternalErrorException("Server::removeClient: Client not found");
		_clients.erase(client.getNickname());
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
			throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name);
		return *(_channels.at(name));
	}

	void	Server::addChannel(Channel *channel)
	{
		if (_channels.find(channel->getName()) != _channels.end())
			throw InternalErrorException("Server::addChannel: Channel already exists");
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
		_pollfds[_pollfds.size() - 1].fd = pollfd.fd;
		_pollfds[_pollfds.size() - 1].events = POLLIN;
		_pollfds[_pollfds.size() - 1].revents = 0;
	}

	void	Server::removePollfd(const pollfd pollfd)
	{
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].fd == pollfd.fd)
			{
				_pollfds.erase(_pollfds.begin() + i);
				break;
			}
		}
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

	bool Server::isClientConnected(const string &nickname) const
	{
		return _clients.find(nickname) != _clients.end();
	}

	void Server::handleClient(Client *client, size_t *i)
	{
		char buffer[BUFFER_SIZE] = {0};

		if (!client)
			throw InternalErrorException("Server::handleClient: Client not found");

		try
		{
			int bytes_read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
			if (bytes_read > 0)
			{
				_handler.setClient(*client);
				_handler.processInput(buffer); // se tutto va bene esegue il comando
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
		catch (const InternalErrorException &e)
		{
			throw;
		}
		catch (const ProtocolErrorException &e)
		{
			client->receiveMessage(e.getContent());
			//logger.log("Error: " + string(e.what()));
		}
		catch (const std::exception &e)
		{
			//logger.log("Unknown error: " + string(e.what()));
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

	void Server::configureNonBlocking(const int socket) const
	{
		int flags;

		flags = fcntl(socket, F_GETFL); //TODO aggiungere fcntl a SystemCalls (probabilmente variadic function)
		if (flags == -1)
			throw std::runtime_error(strerror(errno));
		fcntl(socket, F_SETFL, flags | O_NONBLOCK);
		if (flags == -1)
			throw std::runtime_error(strerror(errno));
	}
}