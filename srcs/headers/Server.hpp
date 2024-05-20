/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 17:48:11 by craimond         ###   ########.fr       */
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

# include "EventHandler.hpp"
# include "SystemCalls.hpp"

using namespace std;

class User;
class Channel;
class Client;

class Server
{
	public:
		explicit Server(const uint16_t port_no, const string &password);
		Server(const Server &copy);
		~Server(void);

		uint16_t							getPort(void) const;
		const string						&getPwdHash(void) const;
		const map<string, Client *>			&getClients(void) const;
		void								setClients(const map<string, Client *> &clients);
		const Client						&getClient(const int socket) const;
		const Client						&getClient(const string &nickname) const;
		void								addClient(Client *client);
		void								removeClient(const Client &client);
		const map<string, Channel *>		&getChannels(void) const;
		void								setChannels(const map<string, Channel *> &channels);
		const Channel						&getChannel(const string &name) const;
		void								addChannel(Channel *channel);
		void								removeChannel(const Channel &channel);
		const vector<pollfd>				&getPollfds(void);
		void								setPollfds(const vector<pollfd> &pollfds);
		void								addPollfd(const pollfd pollfd);
		void								removePollfd(const pollfd pollfd);
		void								removePollfd(const int socket);
		int									getSocket(void) const;
		//setEventHandler e getEventHandler non servono perche' EventHandler e' un singleton

		void								handleClient(Client *client, size_t *i);
		void								disconnectClient(Client *client);
		void								handshake(const int client_socket) const;
		void								configureNonBlocking(const int client_socket) const;
		void								run(void);

		static const map<uint16_t, string>	reply_codes;

		class								ChannelAlreadyExistsException; //addChannel
		class								ChannelNotFoundException; //getChannel, Client::joinChannel
		class 								InvalidPasswordException; //constructor
		class								ClientNotFoundException; //removeClient, getClient
		class								ClientAlreadyExistsException; //addClient
		class 								HandshakeFailedException; //handshake


	private:
	
		const uint16_t						_port; //la porta va da 0 a 65535 (2 bytes)
		const string						_pwd_hash; //la password che serve a qualsiasi Client per accedere a questo server
		map<string, Client *>				_clients; //{nickname, Client *}
		map<string, Channel *>				_channels; // {channel_name, Channel *}
		vector<pollfd>						_pollfds; //TODO usare map invece che vector, mapparli con il socket
		const int							_socket;
		EventHandler						_event_handler;
};

class Server::ChannelAlreadyExistsException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Server::ChannelNotFoundException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Server::InvalidPasswordException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Server::ClientNotFoundException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Server::ClientAlreadyExistsException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Server::HandshakeFailedException : public exception
{
	public:
		virtual const char *what(void) const throw();
};

#endif