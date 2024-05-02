/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 14:44:15 by craimond         ###   ########.fr       */
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
		void	addChannel(const Channel &channel);

	private:
		uint16_t								_port_no; //la porta va da 0 a 65535 (2 bytes)
		size_t									_pwd_hash; //la password che serve a qualsiasi user per accedere a questo server
		vector<Client *>						_clients;
		unordered_map<string, const Channel *>	_channels;
		vector<pollfd>							_pollfds;
		
};

#endif