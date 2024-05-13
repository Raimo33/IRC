/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 16:14:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

#include "Message.hpp"

class User;
class Channel;

class PrivateMessage: public Message
{
	public:
		PrivateMessage();
		PrivateMessage(const string &content, const User &sender, const User &receiver);
		PrivateMessage(const string &content, const User &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage();
		PrivateMessage	&operator=(const PrivateMessage &rhs);
		const User		*getReceiver() const;
	private:
		const User		*_receiver;
};

#endif