/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:17:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

using namespace std;

class User;

class IMessage
{
	public:
		explicit IMessage(const string &content, User &sender);
		IMessage(const IMessage &copy);
		virtual ~IMessage(void) = 0;

		const string	&getContent(void) const;
		time_t 			getTimestamp(void) const;
		const User		&getSender(void) const;

	protected:
		const string	_content;
		const time_t	_timestamp;
		const User		*_sender;
};

#endif