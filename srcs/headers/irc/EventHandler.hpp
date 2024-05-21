/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:42:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>
# include <map>

# include "utils.hpp"

//esiste un event handler per ogni client, che si occupa di gestire i comandi ricevuti dal client
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
			EventHandler(const EventHandler &copy);
			~EventHandler(void);

			const std::map<std::string, e_cmd_type>	&getCommands(void) const;
			const Client							*getClient(void) const;
			void									setClient(Client *client);
			const Server							*getServer(void) const;
			void									setServer(Server *server);

			void 									processInput(std::string raw_input);

			static void								sendBufferedString(const Client &receiver, const std::string &string);

		private:

			s_input									parseInput(std::string &raw_input) const;
			void									executeCommandPrivmsg(const std::vector<std::string> &params); //chiama sendMessage di User
			void									executeCommandMode(const std::vector<std::string> &params);
			void									executeCommandJoin(const std::vector<std::string> &params);
			void									executeCommandPass(const std::vector<std::string> &params);
			void									executeCommandNick(const std::vector<std::string> &params);
			void									executeCommandQuit(const std::vector<std::string> &params);
			void									executeCommandUser(const std::vector<std::string> &params);
			const std::map<std::string, e_cmd_type>	&initCommandMap(void) const;
			void									checkNicknameValidity(const std::string &nickname) const;


			const std::map<std::string, e_cmd_type>	_commands; //TODO spostare fuori (pseudo globale) altrimenti viene creato per ogni server
			Client									*_client;
			Server									*_server;
	};
}
	

#endif