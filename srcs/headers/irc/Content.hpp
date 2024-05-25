/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Content.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:38:24 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 15:46:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENT_HPP
# define CONTENT_HPP

# include <string>
# include <vector>

namespace irc
{
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

	struct s_contentBase
	{
		std::string					prefix;

		std::vector<std::string>	params;
		std::string					text;
		
		virtual ~s_contentBase(void) {};
	};

	struct s_commandContent : public s_contentBase //messaggio in arrivo (comando) oppure PRIVMSG in uscita
	{
		enum e_cmd_type	cmd;
	};

	struct s_replyContent : public s_contentBase //messaggio di output (reply)
	{
		uint16_t		code;
	};
}

#endif