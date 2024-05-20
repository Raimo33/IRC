/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 17:04:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Server.hpp"

static void	check_args(const int argc, const char **argv);
static void	get_args(uint16_t *port_nbr, string *password, const char **argv);

//primo argv e' la porta, il secondo e' la password
int main(const int argc, const char **argv)
{
	try
	{
		uint16_t	port_nbr;
		string		password;

		check_args(argc, argv);
		get_args(&port_nbr, &password, argv);

		Server	server(port_nbr, password);

		server.run();
	}
	catch (const exception &e)
	{
		cerr << e.what() << endl;
	}
}

static void	check_args(const int argc, const char **argv)
{
	if (argc != 3)
		throw runtime_error("Usage: ./irc <port> <password>");
	(void)argv;
}

static void	get_args(uint16_t *port_nbr, string *password, const char **argv)
{
	*port_nbr = atoi(argv[1]);
	//TODO controllare lunghezza password (MAX_SERVER_PASSWORD_LEN)
	*password = argv[2];
}