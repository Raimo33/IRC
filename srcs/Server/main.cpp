/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/07/03 17:51:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"
#include "SignalHandler.hpp"

using std::cerr;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::string;

static void get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv);

int main(const int argc, const char **argv)
{
	Logger logger;

	try
	{
		uint16_t port_nbr;
		string	 password;

		get_args(&port_nbr, &password, argc, argv);
		SignalHandler signal_handler;

		logger.setFile("ircserv.log");
		Server server(logger, port_nbr, password);
		server.run();
		server.stop();
	}
	catch (const std::invalid_argument &e)
	{
		cerr << "Invalid argument: " << e.what() << endl;
	}
	catch (const std::exception &e)
	{
		logger.logError(&e);
	}
	return (1);
}

static void get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Usage: ./irc <port> <password>");

	istringstream iss(argv[1]);

	if (!(iss >> *port_nbr))
		throw std::invalid_argument("Invalid port number format");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *password))
		throw std::invalid_argument("Invalid password format");
	if (password->length() > MAX_SERVER_PASSWORD_LEN)
	{
		ostringstream oss;

		oss << "Password too long (max " << MAX_SERVER_PASSWORD_LEN << " characters)";
		throw std::invalid_argument(oss.str());
	}
}