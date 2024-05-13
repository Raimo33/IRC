/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 17:24:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

#include "IMessage.hpp"

class User;
class Channel;

class PrivateMessage: public IMessage
{
	public:
		PrivateMessage();
		PrivateMessage(const string &content, const User &sender, const User &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage();
		PrivateMessage	&operator=(const PrivateMessage &rhs);
		const User		*getReceiver() const;
		void			setReceiver(const User &receiver);
	private:
		const User		*_receiver;
};

#endif