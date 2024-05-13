/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 16:10:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <unordered_map>

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

	private:
		void	addClient(void);
		void	removeClient(void);
		uint16_t								_port; //la porta va da 0 a 65535 (2 bytes)
		size_t									_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		unordered_map<string, size_t>			_credentials; // {username, pwd_hash}
		vector<Client *>						_clients;
		unordered_map<string, const Channel *>	_channels;
		vector<pollfd>							_pollfds;
		int										_socket;
};

#endif