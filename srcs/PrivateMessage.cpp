/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:06:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 14:03:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/PrivateMessage.hpp"

PrivateMessage::PrivateMessage(const string &content, const Client &sender, const Client &receiver) :
	IMessage(content, sender),
	_receiver(&receiver) {}

PrivateMessage::PrivateMessage(const PrivateMessage &copy) :
	IMessage(copy),
	_receiver(copy._receiver) {}

PrivateMessage::~PrivateMessage(void) {}

const Client *PrivateMessage::getReceiver(void) const
{
	return _receiver;
}
