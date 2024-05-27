/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 13:38:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "irc/irc.hpp"

using std::string;
using std::istringstream;
using std::ostringstream;
using std::exception;
using std::cerr;
using std::endl;

using namespace irc;

static void	get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv);

int main(const int argc, const char **argv)
{
	try
	{
		uint16_t	port_nbr;
		string		password;

		get_args(&port_nbr, &password, argc, argv);

		Logger	logger(LOG_FILENAME);
		Server	server(logger, port_nbr, password);

		server.run();
	}
	catch (const exception &e)
	{
		cerr << "Shutting down: " << e.what() << endl;
		return (1);
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