/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:06:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 12:00:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/PrivateMessage.hpp"

using std::string;

namespace irc
{
	PrivateMessage::PrivateMessage(const string &text, const Client &sender, const Client &receiver) :
		AMessage(text, sender),
		_receiver(&receiver) {}

	PrivateMessage::PrivateMessage(const PrivateMessage &copy) :
		AMessage(copy),
		_receiver(copy._receiver) {}

	PrivateMessage::~PrivateMessage(void) {}

	const Client *PrivateMessage::getReceiver(void) const
	{
		return _receiver;
	}
}