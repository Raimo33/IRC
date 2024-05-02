/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:35:38 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 15:21:10 by craimond         ###   ########.fr       */
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
		Client(const uint16_t port_no, const uint32_t ip_addr);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &rhs);
		bool		getIsConnected() const;
		uint16_t	getPortNbr() const;
		uint32_t	getIpAddr() const;
		void		setIsConnected(const bool is_connected);
		void		setPortNbr(const uint16_t port_nbr);
		void		setIpAddr(const uint32_t ip_addr);
		
	private:
		string		_host_name; //aka real name
		bool		_is_connected;
		uint16_t	_port_nbr;
		uint32_t	_ip_addr;
		Server		*_server;
};


#endif