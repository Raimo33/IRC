/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:42:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO logging di sistema per errori/eventi importanti (esempio: fallimento di un comando send)
//TODO logging silenzioso per exceptions non fondamentali, esempioo: un utente prova a fare leaveChannel di un canale dove non e' presente.
//TODO mettere i nomi delle eccezioni come i nomi degli errori ufficiali

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
# include "Standards.hpp"
# include "EventHandler.hpp"

namespace irc
{
	class Client;
	class Channel;
	class EventHandler;
	
	class Server
	{
		public:
			explicit Server(const uint16_t port_no, const std::string &password);
			Server(const Server &copy);
			~Server(void);

			uint16_t										getPort(void) const;
			const std::string								&getPwdHash(void) const;
			const std::map<std::string, Client *>			&getClients(void) const;
			void											setClients(const std::map<std::string, Client *> &clients);
			const Client									&getClient(const int socket) const;
			const Client									&getClient(const std::string &nickname) const;
			void											addClient(Client *client);
			void											removeClient(const Client &client);
			const std::map<std::string, Channel *>			&getChannels(void) const;
			void											setChannels(const std::map<std::string, Channel *> &channels);
			const Channel									&getChannel(const std::string &name) const;
			void											addChannel(Channel *channel);
			void											removeChannel(const Channel &channel);
			const std::vector<pollfd>							&getPollfds(void);
			void											setPollfds(const std::vector<pollfd> &pollfds);
			void											addPollfd(const pollfd pollfd);
			void											removePollfd(const pollfd pollfd);
			void											removePollfd(const int socket);
			int												getSocket(void) const;
			//setEventHandler e getEventHandler non servono perche' EventHandler e' un singleton

			void											handleClient(Client *client, size_t *i);
			void											disconnectClient(Client *client);
			void											handshake(const int client_socket) const;
			void											configureNonBlocking(const int client_socket) const;
			void											run(void);

			static const std::map<uint16_t, std::string>	reply_codes;

		private:
			static const std::map<uint16_t, std::string>	initReplyCodes(void);

			const uint16_t									_port; //la porta va da 0 a 65535 (2 bytes)
			const std::string								_pwd_hash; //la password che serve a qualsiasi Client per accedere a questo server
			std::map<std::string, Client *>					_clients; //{nickname, Client *}
			std::map<std::string, Channel *>				_channels; // {channel_name, Channel *}
			std::vector<pollfd>								_pollfds; //TODO usare map invece che std::vector, mapparli con il socket
			const int										_socket;
			EventHandler									_event_handler;
	};
}

#endif