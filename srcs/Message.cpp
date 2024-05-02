/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:59:00 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 13:03:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Message.hpp"

Message::Message() :
	_content(""),
	_timestamp(0),
	_sender(NULL) {}
	
Message::Message(const string &content, const User *sender) :
	_content(content),
	_timestamp(time(NULL)),
	_sender(sender) {}

Message::Message(const Message &copy) :
	_content(copy._content),
	_timestamp(copy._timestamp),
	_sender(copy._sender) {}

Message::~Message() {}