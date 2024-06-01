/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 11:00:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <errno.h>
#include <cstring>

#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"
#include "irc/Message.hpp"
#include "irc/EventHandler.hpp"

using std::string;
using std::map;
using std::vector;

SystemErrorException::SystemErrorException(const string &msg) : runtime_error(msg) {}

SystemErrorException::SystemErrorException(const int errnum) : runtime_error(strerror(errnum)) {}

const char *SystemErrorException::what(void) const throw()
{
	return runtime_error::what();
}

InternalErrorException::InternalErrorException(const string &msg) : runtime_error(msg) {}

const char *InternalErrorException::what(void) const throw()
{
	return runtime_error::what();
}

ProtocolErrorException::ProtocolErrorException(const int value, ...)
{
	va_list	args;

	if (value < RPL_WELCOME || value > ERR_CHANOPRIVSNEEDED)
		throw InternalErrorException("ProtocolErrorException::ProtocolErrorException: Invalid reply code");
	va_start(args, value);
	_content = Message(SERVER_NAME, value, args);
	va_end(args);
}

ProtocolErrorException::~ProtocolErrorException(void) throw() {}

const char *ProtocolErrorException::what(void) const throw()
{
	return _content.getParams().back().c_str();
}

Message	&ProtocolErrorException::getContent(void)
{
	return _content;
}