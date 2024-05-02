/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 14:44:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

Server::Server() : 
	_port_no(DEFAULT_PORT), //default port
	_clients(),
	_channels(),
	_pollfds()
{
	hash<string>	hasher;

	_pwd_hash = hasher(PASSWORD);
}

Server::Server(const uint16_t port_no) :
	_port_no(port_no),
	_clients(),
	_channels(),
	_pollfds()
{
	hash<string>	hasher;

	_pwd_hash = hasher(PASSWORD);
}

Server::Server(const Server &copy) :
	_pwd_hash(copy._pwd_hash),
	_port_no(copy._port_no),
	_clients(copy._clients),
	_channels(copy._channels),
	_pollfds(copy._pollfds) {}

Server::~Server() {}

Server	&Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		_pwd_hash = rhs._pwd_hash;
		_port_no = rhs._port_no;
		_clients = rhs._clients;
		_channels = rhs._channels;
		_pollfds = rhs._pollfds;
	}
	return *this;
}

void	Server::addChannel(const Channel &channel)
{
	_channels[channel.getName()] = &channel;
}