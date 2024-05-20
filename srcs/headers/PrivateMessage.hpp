/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 14:57:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

#include "IMessage.hpp"

class Client;
class Channel;

class PrivateMessage: public IMessage
{
	public:
		explicit PrivateMessage(const string &content, const Client &sender, const Client &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage(void);

		const Client	*getReceiver(void) const;

	private:
		const Client	*_receiver;
};

#endif