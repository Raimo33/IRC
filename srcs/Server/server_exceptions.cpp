/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exceptions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:04:16 by egualand          #+#    #+#             */
/*   Updated: 2024/06/02 17:11:56 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReplyMessage.hpp"
#include "server_exceptions.hpp"
#include "common_exceptions.hpp"
#include "server_constants.hpp"

ProtocolErrorException::ProtocolErrorException(const int reply_code, ...)
{
	va_list args;

	if (reply_code < RPL_WELCOME || reply_code > ERR_CHANOPRIVSNEEDED)
		throw InternalErrorException("ProtocolErrorException::ProtocolErrorException: Invalid reply code");
	va_start(args, reply_code);
	_content = ReplyMessage(SERVER_NAME, reply_code, args);
	va_end(args);
}

ProtocolErrorException::~ProtocolErrorException(void) throw() {}

const char *ProtocolErrorException::what(void) const throw()
{
	return _content.getParams().back().c_str();
}

ReplyMessage &ProtocolErrorException::getContent(void)
{
	return _content;
}