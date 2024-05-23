/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 13:56:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO logging di sistema su piu livelli. azioni degli utenti, errori di sistema, errori di protocollo
//TODO anche i messaggi  replycodes standard utilizzano i parametri e non sono costanti
//TODO implementare scambio di file (ERR_FILEERROR)

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

	//TODO valutare se spostare questa dichiarazione in un file a parte
	struct s_messageBase
	{
		std::string					prefix;
		std::vector<std::string>	parameters;
		std::string					text;

		virtual ~s_messageBase(void) = 0;
	};

	struct s_commandMessage : public s_messageBase //messaggio in arrivo (comando) oppure PRIVMSG in uscita
	{
		std::string	command;
	};

	struct s_replyMessage : public s_messageBase //messaggio di output (reply)
	{
		std::string	code;
	};

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