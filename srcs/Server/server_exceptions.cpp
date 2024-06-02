/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exceptions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:04:16 by egualand          #+#    #+#             */
/*   Updated: 2024/06/02 18:17:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReplyMessage.hpp"
#include "server_exceptions.hpp"
#include "common_exceptions.hpp"
#include "server_constants.hpp"

ActionFailedException::ActionFailedException(const int reply_code, ...)
{
	va_list args;

	if (reply_code < RPL_WELCOME || reply_code > ERR_CHANOPRIVSNEEDED)
		throw InternalErrorException("ActionFailedException::ActionFailedException: Invalid reply code");
	va_start(args, reply_code);
	_content = ReplyMessage(SERVER_NAME, reply_code, args);
	va_end(args);
}

ActionFailedException::~ActionFailedException(void) throw() {}

const char *ActionFailedException::what(void) const throw()
{
	return _content.getParams().back().c_str();
}

ReplyMessage &ActionFailedException::getContent(void)
{
	return _content;
}