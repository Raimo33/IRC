/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 18:44:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO classe logger di sistema su piu livelli. azioni degli utenti, errori di sistema, errori di protocollo
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

namespace irc
{
	class Client;
	class Channel;

	class Server
	{
		public:
			explicit Server(const uint16_t port_no, const std::string &password);
			Server(const Server &copy);
			~Server(void);

			uint16_t								getPort(void) const;
			const std::string						&getPwdHash(void) const;
			const std::map<std::string, Client *>	&getClients(void) const;
			void									setClients(const std::map<std::string, Client *> &clients);
			const Client							&getClient(const int socket) const;
			const Client							&getClient(const std::string &nickname) const;
			void									addClient(Client *client);
			void									removeClient(const Client &client);
			const std::map<std::string, Channel *>	&getChannels(void) const;
			void									setChannels(const std::map<std::string, Channel *> &channels);
			const Channel							&getChannel(const std::string &name) const;
			void									addChannel(Channel *channel);
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

		private:
					
			// static const std::map<uint16_t, std::string>	initReplyCodes(void);

			const uint16_t							_port; //la porta va da 0 a 65535 (2 bytes)
			const std::string						_pwd_hash; //la password che serve a qualsiasi Client per accedere a questo server
			std::map<std::string, Client *>			_clients; //{nickname, Client *}
			std::map<std::string, Channel *>		_channels; // {channel_name, Channel *}
			std::vector<pollfd>						_pollfds; //TODO usare map invece che std::vector, mapparli con il socket
			const int								_socket;
			EventHandler							_handler;
			
	};
}

#endif