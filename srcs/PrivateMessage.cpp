/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:06:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/11 16:20:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/PrivateMessage.hpp"

PrivateMessage::PrivateMessage() : Message(), _receiver(NULL) {}

PrivateMessage::PrivateMessage(const t_msg &content, const User &sender, const User &receiver) :
	Message(content, sender, Channel()),
	_receiver(&receiver) {}

PrivateMessage::PrivateMessage(const Message &message, const User &receiver) :
	Message(message),
	_receiver(&receiver) {}

PrivateMessage::PrivateMessage(const PrivateMessage &copy) :
	Message(copy),
	_receiver(copy._receiver) {}

PrivateMessage::~PrivateMessage() {}