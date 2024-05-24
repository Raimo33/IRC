/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:59:00 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 11:56:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Message.hpp"
	
using std::string;

namespace irc
{
	Message::Message(const std::string &text, const Client &sender, const Channel &channel) :
		AMessage(text, sender),
		_channel(&channel) {}

	Message::Message(const Message &copy) :
		AMessage(copy),
		_channel(copy._channel) {}

	Message::~Message(void) {}

	const Channel *Message::getChannel(void) const
	{
		return _channel;
	}
}