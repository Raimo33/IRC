/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:35:38 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 13:03:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstring>
# include <string>
# include <vector>
# include <errno.h>
# include "Hasher.hpp"

# include "User.hpp"

using namespace std;

class Server;

class Client : public User
{
	public:
		Client(const Server *server, const int socket, const string &ip_addr, const uint16_t port);
		Client(const Client &copy);
		~Client(void);
		bool			getIsConnected(void) const;
		uint16_t		getPort(void) const;
		string			getIpAddr(void) const;
		int				getSocket(void) const;
		Server			*getServer(void) const;
		void			setIsConnected(const bool is_connected);
		void			authenticate(void);

		class			AlreadyConnectedException; //setIsConnected
		class 			NotConnectedException; //EventHandler
		class			InvalidPasswordException; //authenticate

	private:
		bool			_is_connected;
		const uint16_t	_port;
		const string	_ip_addr; //aka hostname
		const int		_socket;
		const Server	*_server;
};

#endif