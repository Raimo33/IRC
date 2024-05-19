/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:23:39 by craimond         ###   ########.fr       */
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
		explicit PrivateMessage(const string &content, const User &sender, const User &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage(void);

		const User	*getReceiver(void) const;

	private:
		const User	*_receiver;
};

#endif