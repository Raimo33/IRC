/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:59:00 by craimond          #+#    #+#             */
/*   Updated: 2024/05/11 16:19:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Message.hpp"

Message::Message() :
	_content((t_msg){0}),
	_timestamp(0),
	_sender(NULL) {}
	
Message::Message(const t_msg &content, const User &sender, const Channel &channel) :
	_content(content),
	_timestamp(time(NULL)),
	_sender(&sender),
	_channel(&channel) {}

Message::Message(const Message &copy) :
	_content(copy._content),
	_timestamp(copy._timestamp),
	_sender(copy._sender) {}

Message::~Message() {}

t_msg	Message::getContent() const
{
	return _content;
}

time_t	Message::getTimestamp() const
{
	return _timestamp;
}

const User	*Message::getSender() const
{
	return _sender;
}