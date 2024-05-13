/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:35:38 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 18:29:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "User.hpp"
# include "Server.hpp"

class Client : public User
{
	public:
		Client();
		Client(const int socket);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &rhs);
		bool		getIsConnected() const;
		uint16_t	getPortNbr() const;
		uint32_t	getIpAddr() const;
		int			getSocket() const;
		Server		*getServer() const;
		void		setIsConnected(const bool is_connected);
		void		setPortNbr(const uint16_t port);
		void		setIpAddr(const uint32_t ip_addr);
		void		setSocket(const int socket);
		void		setServer(Server *server);
		void		run(void);
		class		UnknownCommandException;
		class 		NotConnectedException;
	private:
		void		checkConnection(void) const;
		t_input		parseInput(const string &input) const;
		void		processInput(void); //forse const
		void		authenticate(void);
		string		_host_name; //aka real name
		bool		_is_connected;
		uint32_t	_ip_addr;
		int			_socket;
		Server		*_server;
};

typedef struct s_msg
{
	string			prefix;
	t_cmd			command;
	vector<string>	params;
}	t_input;

typedef enum e_cmd
{
	NICK,
	USER,
	JOIN,
	PRIVMSG,
	QUIT,
}	t_cmd;


#endif