/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:42:23 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 12:44:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

Client::Client() :
	User(),
	_is_connected(false),
	_port_no(0),
	_ip_addr(0) {}

Client::Client(const uint16_t port_no, const uint32_t ip_addr) :
	User(),
	_is_connected(true),
	_port_no(port_no),
	_ip_addr(ip_addr) {}

Client::Client(const Client &copy) :
	User(copy),
	_is_connected(copy._is_connected),
	_port_no(copy._port_no),
	_ip_addr(copy._ip_addr) {}

Client::~Client() {}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		User::operator=(rhs);
		_is_connected = rhs._is_connected;
		_port_no = rhs._port_no;
		_ip_addr = rhs._ip_addr;
	}
	return *this;
}

bool	Client::getIsConnected() const
{
	return _is_connected;
}

uint16_t	Client::getPortNo() const
{
	return _port_no;
}

uint32_t	Client::getIpAddr() const
{
	return _ip_addr;
}

void	Client::setIsConnected(const bool is_connected)
{
	_is_connected = is_connected;
}

void	Client::setPortNo(const uint16_t port_no)
{
	_port_no = port_no;
}

void	Client::setIpAddr(const uint32_t ip_addr)
{
	_ip_addr = ip_addr;
}