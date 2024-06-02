/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 19:12:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "BeetleBot.hpp"
#include "Logger.hpp"
#include "bot_constants.hpp"

using std::string;
using std::istringstream;
using std::ostringstream;
using std::cerr;
using std::endl;

static void	get_args(string *ip, uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv);

int main(const int argc, const char **argv)
{
	Logger	logger;

	try
	{
		string		ip;
		uint16_t	port_nbr;
		string		password;

		get_args(&ip, &port_nbr, &password, argc, argv);

		BeetleBot bot("BeetBot", "BeetleBot", "Beetle Juice Bot");
		logger.setFile(bot.getNickname() + ".log");
		bot.setLogger(logger);
		bot.run();
	}
	catch (const std::invalid_argument &e)
	{
		cerr << "Invalid argument: " << e.what() << endl;
	}
	catch (const std::exception &e)
	{
		logger.logError(&e);
		logger.logEvent("Bot shutting down");
	}
	return (1);
}

static void	get_args(string *ip, uint16_t *port_nbr, string *password, const uint32_t argc, const char **argv)
{	
	if (argc < 3 || argc > 4)
	{
		ostringstream oss;
		oss << "Usage: " << argv[0] << " <ip> <port> [<password>]";
		throw std::invalid_argument(oss.str());
	}

	istringstream	iss(argv[1]);

	if (!(iss >> *ip))
		throw std::invalid_argument("Invalid IP address format");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *port_nbr))
		throw std::invalid_argument("Invalid port number format");
	if (argc == 3)
		return ;
	iss.clear();
	iss.str(argv[3]);
	if (!(iss >> *password))
		throw std::invalid_argument("Invalid password format");
}