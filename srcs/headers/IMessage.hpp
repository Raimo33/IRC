/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:25:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

# include "irc.hpp"

using namespace std;
using namespace irc;
class IMessage
{
	public:
		explicit IMessage(const string &content, const Client &sender);
		IMessage(const IMessage &copy);
		virtual ~IMessage(void) = 0;

		const string	&getContent(void) const;
		time_t 			getTimestamp(void) const;
		const Client	&getSender(void) const;

	protected:
		const string	_content;
		const time_t	_timestamp;
		const Client	*_sender;
};

#endif