/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:09:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 17:39:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/AMessage.hpp"

using namespace std;

namespace irc
{
	AMessage::AMessage(const string &text, const Client &sender) :
		_text(text),
		_timestamp(time(NULL)),
		_sender(&sender) {}

	AMessage::AMessage(const AMessage &copy) :
		_text(copy._text),
		_timestamp(copy._timestamp),
		_sender(copy._sender) {}

	AMessage::~AMessage(void) {}

	const Client	&AMessage::getSender(void) const
	{
		return *_sender;
	}

	const string	&AMessage::getText(void) const
	{
		return _text;
	}

	time_t	AMessage::getTimestamp(void) const
	{
		return _timestamp;
	}
}

