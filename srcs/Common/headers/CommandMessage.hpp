/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:37:03 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 19:05:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMESSAGE_HPP
# define COMMANDMESSAGE_HPP

# include "AMessage.hpp"

# include <string>
# include <map>

enum e_commands
{
	PASS,
	NICK,
	USER,
	JOIN,
	PART,
	PRIVMSG,
	QUIT,
	SEND,
	KICK,
	INVITE,
	TOPIC,
	MODE,
};

class CommandMessage : public AMessage
{
	public:
		CommandMessage(void);
		CommandMessage(const std::string &raw_input);
		CommandMessage(const std::string &prefix, const enum e_commands command, ...);
		CommandMessage(const std::string &prefix, const enum e_commands command, va_list args);
		CommandMessage(const CommandMessage &copy);
		virtual ~CommandMessage(void);

		CommandMessage			&operator=(const CommandMessage &copy);

		void					setCommand(const enum e_commands command);
		const enum e_commands	&getCommand(void) const;

	private:
		void					parse(std::string raw_input);

		enum e_commands			_command;
};

const std::map<std::string, enum e_commands>		create_commands_map(void); //TODO mettere statici dentro la classe
extern const std::map<std::string, enum e_commands>	g_commands;

#endif