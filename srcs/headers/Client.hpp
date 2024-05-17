/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:35:38 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 15:46:00 by craimond         ###   ########.fr       */
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

typedef enum e_cmd
{
	PRIVMSG,
	JOIN,
	MODE,
	PASS,
	NICK,
	USER,
	QUIT
}	t_cmd;

typedef struct s_input
{
	string			prefix;
	t_cmd			command;
	vector<string>	params;
}	t_input;

class Server;

class Client : public User
{
	public:
		Client(void);
		Client(const int socket);
		Client(const Client &copy);
		~Client(void);
		Client	&operator=(const Client &rhs);
		bool		getIsConnected(void) const;
		uint16_t	getPort(void) const;
		string		getIpAddr(void) const;
		int			getSocket(void) const;
		Server		*getServer(void) const;
		void		setIsConnected(const bool is_connected);
		void		setPort(const uint16_t port);
		void		setIpAddr(const string ip_addr);
		void		setSocket(const int socket);
		void		setServer(Server *server);
		void		checkConnection(void) const;
		void		authenticate(void);
		class 		NotConnectedException;
		class		AlreadyConnectedException;
		class		InvalidPasswordException;
	private:
		bool		_is_connected;
		uint16_t	_port;
		string		_ip_addr; //aka hostname
		int			_socket;
		Server		*_server;
};

class Client::NotConnectedException: public exception
{
	public:
		virtual const char	*what() const throw();
};

class Client::AlreadyConnectedException: public exception
{
	public:
		virtual const char	*what() const throw();
};

class Client::InvalidPasswordException: public exception
{
	public:
		virtual const char	*what() const throw();
};

#endif