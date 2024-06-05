/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 16:12:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO Bypassing the Password: When a user is invited to a private channel, they can join the channel without needing to provide the channel password. The invitation overrides the password requirement. This mechanism is especially useful for channels that are set with an invite-only mode (+i) or have a password set (+k).

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>

#include "EventHandler.hpp"
#include "Logger.hpp"
#include "server_constants.hpp"
#include "system_calls.hpp"

class Client;
class Channel;
class Logger;

class Server
{
  public:
	explicit Server(Logger &logger, const uint16_t port_no, const std::string &password);
	Server(const Server &copy);
	~Server(void);

	uint16_t								getPort(void) const;
	const std::string					   &getPwdHash(void) const;
	const std::map<std::string, Client *>  &getClients(void) const;
	void									setClients(const std::map<std::string, Client *> &clients);
	Client								   &getClient(const std::string &nickname) const;
	Client								   *getClient(const int socket) const;
	void									addClient(Client &client);
	void									removeClient(const Client &client);
	const std::map<std::string, Channel *> &getChannels(void) const;
	void									setChannels(const std::map<std::string, Channel *> &channels);
	Channel								   &getChannel(const std::string &name) const;
	void									addChannel(Channel &channel);
	void									removeChannel(const Channel &channel);
	int										getSocket(void) const;
	bool									isClientConnected(const std::string &nickname) const;
	void									disconnectClient(Client &client);
	void									run(void);
	void									stop(void);

  private:
	void handleNewClient(void);
	void handleClient(const int client_socket);
	void handleClientDisconnect(const int client_socket);

	const uint16_t					 _port;
	const std::string				 _pwd_hash;
	std::map<std::string, Client *>	 _clients; //{pk, client}
	std::map<std::string, Channel *> _channels;
	const int						 _epoll_fd;
	const int						 _socket;
	EventHandler					 _handler;
	Logger							&_logger;
};

#endif