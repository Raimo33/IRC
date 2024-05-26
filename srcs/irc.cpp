/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 19:45:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "irc/irc.hpp"

using std::string;
using std::istringstream;
using std::ostringstream;

using namespace irc;

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
	catch (const std::invalid_argument &e)
	{
		//logger.log("Invalid argument: " + string(e.what()));
		return (EXIT_FAILURE);
	}
	catch (const InternalErrorException &e)
	{
		//logger.log("Internal error: " + string(e.what()));
		return (EXIT_FAILURE);
	}
	catch (const SystemErrorException &e)
	{
		//logger.log("System error: " + string(e.what()));
		return (EXIT_FAILURE);
	}
	catch (const std::exception &e)
	{
		//logger.log("Unknown error: " + string(e.what()));
		return (EXIT_FAILURE);
	}
}

static void	get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Usage: ./irc <port> <password>");

	istringstream	iss(argv[1]);

	if (!(iss >> *port_nbr))
		throw std::invalid_argument("Invalid port number");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *password))
		throw std::invalid_argument("Invalid password");
	if (password->length() > MAX_SERVER_PASSWORD_LEN)
	{
		ostringstream oss;

		oss << "Password too long (max " << MAX_SERVER_PASSWORD_LEN << " characters)";
		throw std::invalid_argument(oss.str());
	}
}