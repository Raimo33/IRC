/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 17:10:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>
# include <map>

# include "utils.hpp"
# include "Constants.hpp"
# include "Messages.hpp"

class Client;
class Server;
class Logger;

class EventHandler
{
	public:
		explicit EventHandler(Logger &logger, Server &server);
		EventHandler(const EventHandler &copy);
		~EventHandler(void);

		const std::map<std::string, e_commands>			&getCommands(void) const;
		const Client									&getClient(void) const;
		void											setClient(Client &client);

		void 											processInput(std::string &raw_input);

		static const struct s_message					buildMessage(const string &prefix, const int value, ...);
		static const struct s_message 					buildMessage(const string &prefix, const int value, va_list args);
		static void										sendBufferedMessage(const Client &receiver, const struct s_message &msg);

	private:

		typedef void (EventHandler::*CommandHandler)(const std::vector<std::string>&);

		static void										unwrapMessage(const struct s_message &msg, string *first_part, string *second_part);

		const std::map<std::string, e_commands>			initCommands(void);
		const std::map<e_commands, CommandHandler>		initHandlers(void);
		static std::map<uint16_t, std::string>			initCommandStrings(void);
		const struct s_message							parseInput(std::string &raw_input) const;
		void											handlePass(const std::vector<std::string> &params);
		void											handleNick(const std::vector<std::string> &params);
		void											handleUser(const std::vector<std::string> &params);
		void											handleJoin(const std::vector<std::string> &params);
		void											handlePart(const std::vector<std::string> &params);
		void											handlePrivmsg(const std::vector<std::string> &params); //chiama sendMessage di User
		void											handleQuit(const std::vector<std::string> &params);
		void											handleKick(const std::vector<std::string> &params);
		void											handleInvite(const std::vector<std::string> &params);
		void											handleTopic(const std::vector<std::string> &params);
		void											handleMode(const std::vector<std::string> &params);
		void											checkNicknameValidity(const std::string &nickname) const;

		Server											*_server;
		Client											*_client;
		const std::map<std::string, e_commands>			_commands;
		const std::map<e_commands, CommandHandler>		_handlers;
		static const std::map<uint16_t, std::string>	_command_strings;
		Logger											&_logger;
};

#endif