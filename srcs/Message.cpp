/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:59:00 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Message.hpp"
	
using namespace std;
using namespace irc;

Message::Message(const string &content, const Client &sender, const Channel &channel) :
	IMessage(content, sender),
	_channel(&channel) {}

Message::Message(const Message &copy) :
	IMessage(copy),
	_channel(copy._channel) {}

Message::~Message(void) {}

const Channel *Message::getChannel(void) const
{
	return _channel;
}
