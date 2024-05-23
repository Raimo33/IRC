/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 15:32:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMESSAGE_HPP
#define AMESSAGE_HPP

#include <string>

namespace irc
{
	class Client;

	//numeric reply: :<prefix> <code> <parameters> :<text>\r\n
	//command: <prefix> <command> <parameters> :<text>\r\n
	
	class AMessage
	{
		public:
			explicit AMessage(const std::string &text, const Client &sender);
			AMessage(const AMessage &copy);
			virtual ~AMessage(void) = 0;

			const std::string	&getText(void) const;
			time_t 				getTimestamp(void) const;
			const Client		&getSender(void) const;

		protected:
			const std::string	_text;
			const time_t		_timestamp;
			const Client		*_sender;
	};
}

#endif