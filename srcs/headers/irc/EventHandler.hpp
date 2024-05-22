/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 03:09:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>
# include <map>

# include "utils.hpp"
# include "Constants.hpp"

namespace irc
{
	class Client;
	class Server;

	enum e_cmd_type
	{
		PASS = 0,
		NICK = 1,
		USER = 2,
		JOIN = 3,
		PRIVMSG = 4,
		MODE = 5,
		QUIT = 6
	};

	struct s_input
	{
		std::string					prefix;
		e_cmd_type					command;
		std::vector<std::string>	params;
	};

	class EventHandler
	{
		public:
			EventHandler(void);
			EventHandler(Server *server);
			EventHandler(const EventHandler &copy);
			~EventHandler(void);

			const std::map<std::string, e_cmd_type>			&getCommands(void) const;
			const Client									*getClient(void) const;
			void											setClient(Client *client);
			const Server									*getServer(void) const;
			void											setServer(Server *server);

			void 											processInput(std::string raw_input);

			static void										sendBufferedString(const Client &receiver, const std::string &string);

		private:

			typedef void (EventHandler::*CommandHandler)(const std::vector<std::string>&);

			void											initHandlers(void);
			const std::map<std::string, e_cmd_type>			initCommands(void);
			s_input											parseInput(std::string &raw_input) const;
			void											handlePrivmsg(const std::vector<std::string> &params); //chiama sendMessage di User
			void											handleMode(const std::vector<std::string> &params);
			void											handleJoin(const std::vector<std::string> &params);
			void											handlePass(const std::vector<std::string> &params);
			void											handleNick(const std::vector<std::string> &params);
			void											handleQuit(const std::vector<std::string> &params);
			void											handleUser(const std::vector<std::string> &params);
			void											checkNicknameValidity(const std::string &nickname) const;

			Server											*_server;
			Client											*_client;
			const std::map<std::string, e_cmd_type>			_commands;
			CommandHandler									_handlers[N_COMMANDS]; //TODO fare const
	};
}
	

#endif