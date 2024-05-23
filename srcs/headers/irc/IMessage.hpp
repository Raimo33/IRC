/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 13:50:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

namespace irc
{
	class Client;

	//numeric reply: :<prefix> <code> <parameters> :<text>\r\n
	//command: <prefix> <command> <parameters> :<text>\r\n
	
	class IMessage
	{
		public:
			explicit IMessage(const std::string &text, const Client &sender);
			IMessage(const IMessage &copy);
			virtual ~IMessage(void) = 0;

			const struct s_message	&getContent(void) const;
			time_t 					getTimestamp(void) const;
			const Client			&getSender(void) const;

		protected:
			const struct s_message 	_content; //TODO Prefix: ":" + server_name + " PRIVMSG " + target + " :" + message
			const time_t			_timestamp;
			const Client			*_sender;
	};
}

#endif