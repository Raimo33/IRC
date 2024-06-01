/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:04:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 19:06:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandMessage.hpp"












g_commands = create_commands_map();

const std::map<std::string, enum e_commands>	create_commands_map(void)
{
	std::map<std::string, enum e_commands>	commands;

	commands["PASS"] = PASS;
	commands["NICK"] = NICK;
	commands["USER"] = USER;
	commands["JOIN"] = JOIN;
	commands["PART"] = PART;
	commands["PRIVMSG"] = PRIVMSG;
	commands["QUIT"] = QUIT;

	commands["KICK"] = KICK;
	commands["INVITE"] = INVITE;
	commands["TOPIC"] = TOPIC;
	commands["MODE"] = MODE;
}