/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:09:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 13:08:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/IMessage.hpp"

IMessage::IMessage(const string &content, User &sender) :
	_content(content),
	_timestamp(time(NULL)),
	_sender(&sender) {}

IMessage::~IMessage(void) {}

const User	&IMessage::getSender(void) const
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

