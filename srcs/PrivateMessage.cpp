/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:06:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 13:16:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/PrivateMessage.hpp"

using namespace std;
using namespace irc;

PrivateMessage::PrivateMessage(const string &text, const Client &sender, const Client &receiver) :
	IMessage(text, sender),
	_receiver(&receiver) {}

PrivateMessage::PrivateMessage(const PrivateMessage &copy) :
	IMessage(copy),
	_receiver(copy._receiver) {}

PrivateMessage::~PrivateMessage(void) {}

const Client *PrivateMessage::getReceiver(void) const
{
	return _receiver;
}
