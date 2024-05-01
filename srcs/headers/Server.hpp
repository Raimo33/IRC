/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:09:02 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 00:27:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <unordered_map>

# define PASSWORD "beetlejuice"

using namespace std;

class Client;
class Channel;

class Server
{
	public:
		Server();
		Server(const Server &copy);
		~Server();
		Server	&operator=(const Server &rhs);
		
	private:
		vector<Client>					_clients;
		unordered_map<string, Channel>	_channels;
		vector<pollfd>					_pollfds;
		uint16_t						_port_no; //la porta va da 0 a 65535 (2 bytes)
		size_t							_pwd_hash; //la password che serve a qualsiasi client per accedere a questo server
		
};

#endif