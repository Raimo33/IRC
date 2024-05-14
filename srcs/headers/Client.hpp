/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:35:38 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 16:55:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstring>
# include <string>
# include <vector>
# include <errno.h>
# include "hasher.hpp"

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
		Client();
		Client(const int socket);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &rhs);
		bool		getIsConnected() const;
		uint32_t	getIpAddr() const;
		int			getSocket() const;
		Server		*getServer() const;
		void		setIsConnected(const bool is_connected);
		void		setIpAddr(const uint32_t ip_addr);
		void		setSocket(const int socket);
		void		setServer(Server *server);
		void		run(void);
		void		checkConnection(void) const;
		void		authenticate(void);
		class 		NotConnectedException;
		class		AlreadyConnectedException;
		class		InvalidPasswordException;
	private:
		bool		_is_connected;
		uint32_t	_ip_addr; //aka hostname
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