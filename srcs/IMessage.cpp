/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:09:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 18:14:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/IMessage.hpp"

IMessage::IMessage(void) :
	_content(""),
	_timestamp(0),
	_sender(NULL) {}

IMessage::IMessage(const string &content, const User &sender) :
	_content(content),
	_timestamp(time(NULL)),
	_sender(&sender) {}

IMessage::~IMessage(void) {}

User	&IMessage::getSender(void) const
{
	return *_sender;
}

string	IMessage::getContent(void) const
{
	return _content;
}

time_t	IMessage::getTimestamp(void) const
{
	return _timestamp;
}

void	IMessage::setContent(const string &content)
{
	_content = content;
}

void	IMessage::setTimestamp(const time_t timestamp)
{
	_timestamp = timestamp;
}

void	IMessage::setSender(User &sender)
{
	_sender = &sender;
}

