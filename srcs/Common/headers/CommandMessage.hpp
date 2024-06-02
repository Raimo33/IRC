/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:37:03 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 18:09:18 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMESSAGE_HPP
# define COMMANDMESSAGE_HPP

# include "AMessage.hpp"

# include <string>
# include <map>

enum e_commands
{
	PASS = 0,
	NICK = 1,
	USER = 2,
	JOIN = 3,
	PART = 4,
	PRIVMSG = 5,
	QUIT = 6,
	SEND = 7,
	KICK = 8,
	INVITE = 9,
	TOPIC = 10,
	MODE = 11,

	CMD_UNKNOWN = -1
};

class CommandMessage : public AMessage
{
	public:
		CommandMessage(void);
		CommandMessage(const std::string &raw_input);
		CommandMessage(const std::string &prefix, int command, ...);
		CommandMessage(const std::string &prefix, const enum e_commands command, va_list args);
		CommandMessage(const CommandMessage &copy);
		virtual ~CommandMessage(void);

		CommandMessage					&operator=(const CommandMessage &copy);

		void							setCommand(const enum e_commands command);
		const enum e_commands			&getCommand(void) const;
		
		static enum e_commands			getCommandFromString(const std::string &command);
		static const std::string		getStringFromCommand(const enum e_commands command);

	private:
		void							parse(std::string raw_input);
		void							unwrapMessage(std::string &first_part, std::string &second_part) const;

		enum e_commands					_command;
};

extern const std::map<std::string, enum e_commands>	g_str_cmd_map;
extern const std::map<enum e_commands, std::string>	g_cmd_str_map;

const std::map<std::string, enum e_commands>	create_str_cmd_map(void);
const std::map<enum e_commands, std::string>	create_cmd_str_map(void);

#endif