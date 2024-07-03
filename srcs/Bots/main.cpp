/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/07/03 17:56:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "BeetleBot.hpp"
#include "Logger.hpp"
#include "SignalHandler.hpp"
#include "bot_constants.hpp"

using std::cerr;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::string;

static void get_args(string *ip, string *port, string *password, const uint32_t argc, const char **argv);

int main(const int argc, const char **argv)
{
	Logger logger;

	try
	{
		string ip;
		string port;
		string password;

		get_args(&ip, &port, &password, argc, argv);
		SignalHandler signal_handler;

		logger.setFile("BeetleBot.log");
		BeetleBot bot("BeetBot", "BeetleBot", "Beetle Juice Bot");
		bot.setLogger(logger);
		bot.bindServer(ip, port, password);
		bot.run();
		bot.stop();
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

static void get_args(string *ip, string *port, string *password, const uint32_t argc, const char **argv)
{
	if (argc < 3 || argc > 4)
	{
		ostringstream oss;
		oss << "Usage: " << argv[0] << " <ip> <port> [<password>]";
		throw std::invalid_argument(oss.str());
	}

	istringstream iss(argv[1]);

	if (!(iss >> *ip))
		throw std::invalid_argument("Invalid IP address format");
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> *port))
		throw std::invalid_argument("Invalid port format");
	if (argc == 3)
		return;
	iss.clear();
	iss.str(argv[3]);
	if (!(iss >> *password))
		throw std::invalid_argument("Invalid password format");
}