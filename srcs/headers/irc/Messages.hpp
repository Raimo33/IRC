/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:38:24 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:33:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENT_HPP
# define CONTENT_HPP

# include <string>
# include <vector>

# include "irc/ReplyCodes.hpp"

enum e_cmd_type
{
	PASS,
	NICK,
	USER,
	JOIN,
	PART,
	PRIVMSG,
	QUIT,
	KICK,
	INVITE,
	TOPIC,
	MODE,
};

struct s_messageBase
{
	std::string					prefix;

	std::vector<std::string>	params;
	
	virtual ~s_messageBase(void) {};
};

struct s_commandMessage : public s_messageBase //messaggio in arrivo (comando) oppure PRIVMSG in uscita
{
	enum e_cmd_type		cmd;
};

struct s_replyMessage : public s_messageBase //messaggio di output (reply)
{
	enum e_replyCodes	code;
};

#endif