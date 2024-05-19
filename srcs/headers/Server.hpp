/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:18:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO logging di sistema per errori/eventi importanti (esempio: fallimento di un comando send)

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <cstring>
# include <map>
# include <poll.h>
# include <netinet/in.h>
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdexcept>
# include <sstream>

# include "SystemCalls.hpp"
# include "Channel.hpp"

# define BUFFER_SIZE 1024
# define MAX_PASSWORD_LEN 64

using namespace std;

class user;
class Client;

class Server
{
	public:
		explicit Server(const uint16_t port_no, const string &password);
		Server(const Server &copy);
		~Server(void);

		void							run(void);

		class							ChannelAlreadyExistsException; //addChannel
		class							ChannelNotFoundException; //getChannel, User::joinChannel
		class							UserAlreadyExistsException; //addUser
		class							UserNotFoundException; //getUserPassword, getUser
		class 							InvalidPasswordException; //constructor
		class							ClientNotFoundException; //removeClient, getClient
		class							ClientAlreadyExistsException; //addClient
		class 							HandshakeFailedException; //handshake

		uint16_t						getPort(void) const;
		size_t							getPwdHash(void) const;
		const vector<Client *>			&getClients(void) const;
		void							setClients(const vector<Client *> &clients);
		const Client					&getClient(const string &username) const;
		void							addClient(Client *client);
		void							removeClient(const Client &client);
		const map<string, User *>		&getUsers(void) const;
		void							setUsers(const map<string, User *> &users);
		const User						&getUser(const string &username) const;
		void							addUser(User *user);
		void							removeUser(const User &user);
		const map<string, Channel *>	&getChannels(void) const;
		void							setChannels(const map<string, Channel *> &channels);
		const Channel					&getChannel(const string &name) const;
		void							addChannel(Channel *channel);
		void							removeChannel(const Channel &channel);
		const vector<pollfd>			&getPollfds(void);
		void							setPollfds(const vector<pollfd> &pollfds);
		void							addPollfd(const pollfd pollfd);
		void							removePollfd(const pollfd pollfd);
		void							removePollfd(const int socket);
		int								getSocket(void) const;

		const string					&getUserPassword(const string &username) const;
		void							handleClient(Client *client, size_t *i);
		void							disconnectClient(Client *client);
		void							handshake(const int client_socket) const;
		void							configureNonBlocking(const int client_socket) const;

	private:
		const uint16_t					_port; //la porta va da 0 a 65535 (2 bytes)
		const size_t					_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		vector<Client *>				_clients;
		map<string, User *>				_users;
		map<string, Channel *>			_channels;
		vector<pollfd>					_pollfds;
		const int						_socket;
};

class Server::ChannelAlreadyExistsException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::ChannelNotFoundException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::UserAlreadyExistsException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::UserNotFoundException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::InvalidPasswordException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::ClientNotFoundException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::ClientAlreadyExistsException : public exception
{
	public:
		virtual const char *what() const throw();
};

class Server::HandshakeFailedException : public exception
{
	public:
		virtual const char *what() const throw();
};

#endif