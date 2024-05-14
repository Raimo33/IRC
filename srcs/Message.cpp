/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:59:00 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 18:09:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Message.hpp"

Message::Message() :
	IMessage(),
	_channel(NULL) {}
	
Message::Message(const string &content, const User &sender, const Channel &channel) :
	IMessage(content, sender),
	_channel(&channel) {}

Message::Message(const Message &copy) :
	IMessage(copy),
	_channel(copy._channel) {}

Message::~Message() {}

Channel &Message::getChannel() const
{
	return *_channel;
}

void Message::setChannel(Channel &channel)
{
	_channel = &channel;
}