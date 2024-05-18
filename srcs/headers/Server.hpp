/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 13:54:08 by craimond         ###   ########.fr       */
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

# include "wrappers.hpp"
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

		void						setup(void);
		void						run(void);

		class						ChannelAlreadyExistsException; //addChannel
		class						ChannelNotFoundException; //getChannel, User::joinChannel
		class						UserAlreadyExistsException; //addUser
		class						UserNotFoundException; //getUserPassword, getUser
		class 						InvalidPasswordException; //constructor
		class						ClientNotFoundException; //removeClient, getClient
		class						ClientAlreadyExistsException; //addClient
		class 						HandshakeFailedException; //handshake
		
	
	private:
		void						addChannel(Channel &channel);
		void						addUser(User &user);
		Channel						&getChannel(const string &name) const;
		size_t						getPwdHash(void) const;
		size_t						getUserPassword(const string &username) const;
		User						&getUser(const string &nickname) const;
		void						removeClient(Client *client);
		Client						&getClient(const User &user) const;
		void						addClient(void);
		void						handleClient(Client *client, size_t *i);
		void						handshake(const int client_socket) const;
		void						configureNonBlocking(const int client_socket) const;

		const uint16_t				_port; //la porta va da 0 a 65535 (2 bytes)
		const size_t				_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		vector<Client *>			_clients;
		map<string, User *>			_users;
		map<string, Channel *>		_channels;
		vector<pollfd>				_pollfds;
		const int					_socket;
};

#endif