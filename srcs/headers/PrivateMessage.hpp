/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 14:55:43 by egualand         ###   ########.fr       */
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
		explicit PrivateMessage(const string &content, User &sender, const User &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage(void);

		const User	*getReceiver() const;

	private:
		const User	*_receiver;
};

#endif