/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/30 20:22:30 by craimond         ###   ########.fr       */
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
#include "irc/Messages.hpp"
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
	_content = EventHandler::buildMessage(SERVER_NAME, value, args);
	va_end(args);
}

ProtocolErrorException::~ProtocolErrorException(void) throw() {}

const char *ProtocolErrorException::what(void) const throw()
{
	return _content.params.back().c_str();
}

const struct s_message	&ProtocolErrorException::getContent(void) const
{
	return _content;
}