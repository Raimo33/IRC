/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:09:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 13:38:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/IMessage.hpp"

using namespace std;
using namespace irc;

IMessage::IMessage(const string &text, const Client &sender) :
	_content() //TODO formare il messaggio
	_content({SERVER_NAME, })
	_timestamp(time(NULL)),
	_sender(&sender) {}

IMessage::IMessage(const IMessage &copy) :
	_content(copy._content), //TODO formare il messaggio
	_timestamp(copy._timestamp),
	_sender(copy._sender) {}

IMessage::~IMessage(void) {}

const Client	&IMessage::getSender(void) const
{
	return *_sender;
}

const struct s_message	&IMessage::getContent(void) const
{
	return _content;
}

time_t	IMessage::getTimestamp(void) const
{
	return _timestamp;
}

