/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 17:46:15 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>
# include <map>

# include "server_utils.hpp"
# include "CommandMessage.hpp"

class Client;
class Server;
class Logger;

class EventHandler
{
	public:
		explicit EventHandler(Logger &logger, Server &server);
		EventHandler(const EventHandler &copy);
		~EventHandler(void);

		const Client									&getClient(void) const;
		void											setClient(Client &client);

		void 											processInput(std::string &raw_input);

	private:
		typedef void (EventHandler::*CommandHandler)(const std::vector<std::string> &);

		const std::map<enum e_commands, CommandHandler>	initHandlers(void);
		static std::map<uint16_t, std::string>			initCommandStrings(void);
		void											handlePass(const std::vector<std::string> &params);
		void											handleNick(const std::vector<std::string> &params);
		void											handleUser(const std::vector<std::string> &params);
		void											handleJoin(const std::vector<std::string> &params);
		void											handlePart(const std::vector<std::string> &params);
		void											handlePrivmsg(const std::vector<std::string> &params);
		void											handleQuit(const std::vector<std::string> &params);
		void											handleSend(const std::vector<std::string> &params);
		void											handleKick(const std::vector<std::string> &params);
		void											handleInvite(const std::vector<std::string> &params);
		void											handleTopic(const std::vector<std::string> &params);
		void											handleMode(const std::vector<std::string> &params);
		void											checkNicknameValidity(const std::string &nickname) const;
		uint16_t										getRandomPort(void) const;

		Server											*_server;
		Client											*_client;
		const std::map<e_commands, CommandHandler>		_handlers;
		static const std::map<uint16_t, std::string>	_command_strings;
		Logger											&_logger;
};

#endif