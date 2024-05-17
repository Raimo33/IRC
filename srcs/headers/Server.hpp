/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 16:14:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# include "wrappers.hpp"
# include "Channel.hpp"

# define BUFFER_SIZE 1024

using namespace std;

class user;
class Client;

class Server
{
	public:
		Server(const uint16_t port_no, const string &password);
		Server(const Server &copy);
		~Server();
		Server	&operator=(const Server &rhs);
		void	setup(void);
		void	run(void);
		void	addChannel(Channel &channel);
		Channel	&getChannel(const string &name) const;
		size_t	getPwdHash(void) const;
		size_t	getUserPassword(const string &username) const;
		Client	&getClient(const string &nickname) const;
		void	removeClient(Client *client);
		class	ClientNotFoundException;
	private:
		void	addClient(void);
		void	handleClient(Client *client, size_t *i);
		void	handshake(const int client_socket) const;
		void	configureNonBlocking(const int client_socket) const;
		uint16_t				_port; //la porta va da 0 a 65535 (2 bytes)
		size_t					_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		map<string, size_t>		_credentials; // {username, pwd_hash}
		vector<Client *>		_clients;
		map<string, Channel *>	_channels;
		vector<pollfd>			_pollfds;
		int						_socket;
};

class Server::ClientNotFoundException : public exception
{
	public:
		virtual const char *what() const throw();
};

#endif