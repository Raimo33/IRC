/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 23:26:53 by craimond         ###   ########.fr       */
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

		const std::map<std::string, e_cmd_type>			&getCommands(void) const;
		const Client									&getClient(void) const;
		void											setClient(Client &client);

		void 											processInput(const std::string raw_input);

		static const struct s_replyMessage				buildReplyMessage(const enum e_replyCodes code, const std::vector<std::string> &params, const std::string &custom_msg = "");
		static const struct s_replyMessage				buildReplyMessage(const enum e_replyCodes code, const std::string &param = "", const std::string &custom_msg = "");
		static const struct s_commandMessage			buildCommandMessage(const std::string &prefix, const e_cmd_type cmd, const std::vector<std::string> &params, const std::string &custom_msg = "");
		static const struct s_commandMessage			buildCommandMessage(const std::string &prefix, const e_cmd_type cmd, const std::string param = "", const std::string &custom_msg = "");
		static void										sendBufferedContent(const Client &receiver, const struct s_messageBase *message);

	private:

		typedef void (EventHandler::*CommandHandler)(const std::vector<std::string>&);

		static void										getRawReplyMessage(const Client &receiver, const struct s_replyMessage *reply, std::string *first, std::string *second);
		static void										getRawCommandMessage(const struct s_commandMessage *command, std::string *first, std::string *second);

		const std::map<std::string, e_cmd_type>			initCommands(void);
		const std::vector<CommandHandler>				initHandlers(void);
		static std::map<uint16_t, std::string>			initCommandStrings(void);
		struct s_commandMessage							parseInput(std::string &raw_input) const;
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
		const std::map<std::string, e_cmd_type>			_commands;
		const std::vector<CommandHandler>				_handlers;
		static const std::map<uint16_t, std::string>	_command_strings;
		Logger											&_logger;
};

#endif