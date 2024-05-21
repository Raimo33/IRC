/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 18:28:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

namespace irc
{
	class Client;
	
	class IMessage
	{
		public:
			explicit IMessage(const std::string &content, const Client &sender);
			IMessage(const IMessage &copy);
			virtual ~IMessage(void) = 0;

			const std::string	&getContent(void) const;
			time_t 				getTimestamp(void) const;
			const Client		&getSender(void) const;

		protected:
			const std::string	_content;
			const time_t		_timestamp;
			const Client		*_sender;
	};
}

#endif