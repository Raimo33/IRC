/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "irc/irc.hpp"

using namespace std;
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
	catch (const InvalidArgumentException &e)
	{
		cerr << "Invalid argument: \n" << e.what() << endl;
		return (EXIT_FAILURE);
	}
	catch (const FatalErrorException &e)
	{
		cerr << "Fatal error: \n" << e.what() << endl;
		return (EXIT_FAILURE);
	}
	catch (const exception &e)
	{
		cerr << "An error occurred: \n" << e.what() << endl;
		return (EXIT_FAILURE);
	}
}

static void	get_args(uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv)
{
	if (argc != 3)
		throw invalid_argument("Usage: ./irc <port> <password>");

	istringstream	iss(argv[1]);

	if (!(iss >> *port_nbr))
		throw invalid_argument("Invalid port number");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *password))
		throw invalid_argument("Invalid password");
	if (password->length() > MAX_SERVER_PASSWORD_LEN)
	{
		ostringstream oss;

		oss << "Password too long (max " << MAX_SERVER_PASSWORD_LEN << " characters)";
		throw invalid_argument(oss.str());
	}
}