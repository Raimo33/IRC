/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 14:56:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

using namespace std;

class Client;

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