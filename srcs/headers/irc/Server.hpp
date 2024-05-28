/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:49:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO implementare scambio di file (ERR_FILEERROR)
//TODO capire la differenza tra <nick>!<user>@<host> e <nick>

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <cstring>
# include <cstdlib>
# include <map>
# include <poll.h>
# include <netinet/in.h>
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdexcept>
# include <sstream>
# include <algorithm>

# include "SystemCalls.hpp"
# include "Constants.hpp"
# include "EventHandler.hpp"
# include "Logger.hpp"

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
		const std::string						&getPwdHash(void) const;
		const std::map<std::string, Client *>	&getClients(void) const;
		void									setClients(const std::map<std::string, Client *> &clients);
		Client									*getClient(const std::string &nickname) const;
		void									addClient(Client *client);
		void									removeClient(const Client &client);
		const std::map<std::string, Channel *>	&getChannels(void) const;
		void									setChannels(const std::map<std::string, Channel *> &channels);
		Channel									*getChannel(const std::string &name) const;
		void									addChannel(Channel &channel);
		void									removeChannel(const Channel &channel);
		const std::vector<pollfd>				&getPollfds(void);
		void									setPollfds(const std::vector<pollfd> &pollfds);
		void									addPollfd(const pollfd pollfd);
		void									removePollfd(const pollfd pollfd);
		void									removePollfd(const int socket);
		int										getSocket(void) const;

		bool									isClientConnected(const std::string &nickname) const;
		void									handleClient(Client *client, size_t *i);
		void									disconnectClient(Client *client);
		void									handshake(const int client_socket) const;
		void									configureNonBlocking(const int client_socket) const;
		void									run(void);
		void									stop(void);

	private:

		const uint16_t							_port;
		const std::string						_pwd_hash;
		std::map<std::string, Client *>			_clients;
		std::map<std::string, Channel *>		_channels;
		std::vector<pollfd>						_pollfds; //TODO usare map invece che std::vector, mapparli con il socket
		const int								_socket;
		EventHandler							_handler;
		Logger									&_logger;
};

#endif