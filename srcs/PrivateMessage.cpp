/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:06:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 16:22:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/PrivateMessage.hpp"

PrivateMessage::PrivateMessage(void) :
	IMessage(),
	_receiver(NULL) {}

PrivateMessage::PrivateMessage(const string &content, User &sender, const User &receiver) :
	IMessage(content, sender),
	_receiver(&receiver) {}

PrivateMessage::PrivateMessage(const PrivateMessage &copy) :
	IMessage(copy),
	_receiver(copy._receiver) {}

PrivateMessage::~PrivateMessage(void) {}

PrivateMessage&	PrivateMessage::operator=(const PrivateMessage &rhs)
{
	if (this != &rhs)
	{
		IMessage::operator=(rhs);
		_receiver = rhs._receiver;
	}
	return *this;
}

const User*	PrivateMessage::getReceiver(void) const
{
	return _receiver;
}

void	PrivateMessage::setReceiver(const User &receiver)
{
	_receiver = &receiver;
}