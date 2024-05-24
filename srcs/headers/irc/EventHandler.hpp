/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 15:31:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>
# include <map>

# include "utils.hpp"
# include "Constants.hpp"
# include "Content.hpp"

namespace irc
{
	class Client;
	class Server;

	class EventHandler
	{
		public:
			EventHandler(void);
			EventHandler(Server &server);
			EventHandler(const EventHandler &copy);
			~EventHandler(void);

			const std::map<std::string, e_cmd_type>			&getCommands(void) const;
			const Client									&getClient(void) const;
			void											setClient(Client &client);
			const Server									&getServer(void) const;
			void											setServer(Server &server);

			void 											processInput(std::string raw_input);

			static const struct s_replyContent				buildReplyContent(const std::string &custom_msg, const uint32_t code, ...);
			static const struct s_commandContent			buildCommandContent(const std::string &prefix, const std::string &custom_msg, const uint32_t cmd, ...);
			static void										sendBufferedContent(const Client &receiver, const struct s_contentBase *message);

		private:

			typedef void (EventHandler::*CommandHandler)(const std::vector<std::string>&);

			static void										getRawReplyMessage(const struct s_replyContent *reply, std::string *first, std::string *second);
			static void										getRawCommandMessage(const struct s_commandContent *command, std::string *first, std::string *second);

			void											initHandlers(void);
			const std::map<std::string, e_cmd_type>			initCommands(void);
			static std::map<uint16_t, std::string>			initCommandStrings(void);
			struct s_commandContent							parseInput(std::string &raw_input) const;
			void											handlePrivmsg(const std::vector<std::string> &params); //chiama sendMessage di User
			void											handleJoin(const std::vector<std::string> &params);
			void											handlePass(const std::vector<std::string> &params);
			void											handleNick(const std::vector<std::string> &params);
			void											handleUser(const std::vector<std::string> &params);
			void											handleQuit(const std::vector<std::string> &params);
			void											handleKick(const std::vector<std::string> &params);
			void											handleInvite(const std::vector<std::string> &params);
			void											handleTopic(const std::vector<std::string> &params);
			void											handleMode(const std::vector<std::string> &params);
			void											checkNicknameValidity(const std::string &nickname) const;

			Server											*_server;
			Client											*_client;
			const std::map<std::string, e_cmd_type>			_commands;
			CommandHandler									_handlers[N_COMMANDS]; //TODO fare const
			static const std::map<uint16_t, std::string>	_command_strings;
	};
}
	

#endif