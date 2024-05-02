/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:55:39 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 16:39:04 by craimond         ###   ########.fr       */
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
		PrivateMessage(const Message &message, const User &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage();
		PrivateMessage	&operator=(const PrivateMessage &rhs);
		const User		*getReceiver() const;
	private:
		const User		*_receiver;
};

#endif