/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 16:36:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO aggiungere bot
//TODO UNKNOWN message non corretto

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>

#include "system_calls.hpp"
#include "server_constants.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"

class Client;
class Channel;
class Logger;

class Server
{
public:
	explicit Server(Logger &logger, const uint16_t port_no, const std::string &password);
	Server(const Server &copy);
	~Server(void);

	uint16_t getPort(void) const;
	const std::string &getPwdHash(void) const;
	const std::map<std::string, Client *> &getClients(void) const;
	void setClients(const std::map<std::string, Client *> &clients);
	Client &getClient(const std::string &nickname) const;
	Client *getClient(const int socket) const;
	void addClient(Client &client);
	void removeClient(const Client &client);
	const std::map<std::string, Channel *> &getChannels(void) const;
	void setChannels(const std::map<std::string, Channel *> &channels);
	Channel &getChannel(const std::string &name) const;
	void addChannel(Channel &channel);
	void removeChannel(const Channel &channel);
	int getSocket(void) const;

	bool isClientConnected(const std::string &nickname) const;
	void disconnectClient(Client &client);
	void configureNonBlocking(const int client_socket) const;
	void run(void);
	void stop(void);

private:
	void handleNewClient(void);
	void handleClient(const int client_socket);
	void handleClientDisconnect(const int client_socket);

	const uint16_t _port;
	const std::string _pwd_hash;
	std::map<std::string, Client *> _clients; //{pk, client}
	std::map<std::string, Channel *> _channels;
	const int _epoll_fd;
	const int _socket;
	EventHandler _handler;
	Logger &_logger;
};

#endif