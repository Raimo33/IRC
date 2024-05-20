/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:09:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 14:56:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/IMessage.hpp"

IMessage::IMessage(const string &content, const Client &sender) :
	_content(content),
	_timestamp(time(NULL)),
	_sender(&sender) {}

IMessage::IMessage(const IMessage &copy) :
	_content(copy._content),
	_timestamp(copy._timestamp),
	_sender(copy._sender) {}

IMessage::~IMessage(void) {}

const Client	&IMessage::getSender(void) const
{
	return *_sender;
}

const string	&IMessage::getContent(void) const
{
	return _content;
}

time_t	IMessage::getTimestamp(void) const
{
	return _timestamp;
}

