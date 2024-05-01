/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 00:23:51 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 00:27:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

Server::Server() : 
	_pwd_hash(hash<string>{}(PASSWORD)),
	_port_no(8080), //default port
	_clients(),
	_channels(),
	_pollfds() {}

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