/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 18:02:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>
# include <poll.h>
# include <netinet/in.h>
# include <iostream>

# include "wrappers.hpp"
# include "Channel.hpp"

# define PASSWORD "beetlejuice"
# define DEFAULT_PORT 8080
# define BUFFER_SIZE 512

using namespace std;

class user;
class Client;

class Server
{
	public:
		Server();
		// Server(uint16_t port_no, string &password);
		Server(const uint16_t port_no);
		Server(const Server &copy);
		~Server();
		Server	&operator=(const Server &rhs);
		void	run(void);
		void	addChannel(const Channel &channel);
		Channel	&getChannel(const string &name) const;
		size_t	getPwdHash(void) const;
		Client	&getClient(const string &username) const;

	private:
		void	addClient(void);
		void	handshake(void);
		void	removeClient(void);
		uint16_t								_port; //la porta va da 0 a 65535 (2 bytes)
		size_t									_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		map<string, size_t>						_credentials; // {username, pwd_hash}
		vector<Client *>						_clients;
		map<string, const Channel *>			_channels;
		vector<pollfd>							_pollfds;
		int										_socket;
};

#endif