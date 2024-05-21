/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 14:11:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

#include <iostream>

static void	check_args(const int argc, const char **argv);
static void	get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv);

int main(const int argc, const char **argv)
{
	try
	{
		uint16_t	port_nbr;
		string		password;

		get_args(&port_nbr, &password, argc, argv);

		Server	server(port_nbr, password);

		server.run();
	}
	catch (const Server::FatalErrorException &e)
	{
		cerr << e.what() << endl;
		return (EXIT_FAILURE);
	}
}

static void	get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv)
{
	istringstream	iss(argv[1]);

	if (argc != 3)
		throw Server::FatalErrorException("Usage: ./irc <port> <password>");
	if (!(iss >> *port_nbr))
		throw Server::FatalErrorException("Invalid port number");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *password))
		throw Server::FatalErrorException("Invalid password");
	if (password->length() > MAX_SERVER_PASSWORD_LEN)
	{
		ostringstream oss;

		oss << "Password too long (max " << MAX_SERVER_PASSWORD_LEN << " characters)";
		throw Server::FatalErrorException(oss.str());
	}
}