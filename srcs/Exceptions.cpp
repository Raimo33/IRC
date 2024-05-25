/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 17:57:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <map>
#include <vector>
#include <string>

#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/EventHandler.hpp"

using std::string;
using std::map;
using std::vector;

namespace irc
{
	SystemErrorException::SystemErrorException(const string &msg) : runtime_error(msg) {}

	const char *SystemErrorException::what(void) const throw()
	{
		return runtime_error::what();
	}

	InternalErrorException::InternalErrorException(const string &msg) : runtime_error(msg) {}

	const char *InternalErrorException::what(void) const throw()
	{
		return runtime_error::what();
	}

	ProtocolErrorException::ProtocolErrorException(const uint16_t code, const string *params, const string &custom_msg) :
		_content(EventHandler::buildReplyContent(code, params, custom_msg)) {}

	ProtocolErrorException::ProtocolErrorException(const uint16_t code, const string param, const string &custom_msg) :
		_content(EventHandler::buildReplyContent(code, param, custom_msg)) {}

	ProtocolErrorException::~ProtocolErrorException(void) throw() {}

	const struct s_replyContent	&ProtocolErrorException::getContent(void) const
	{
		return _content;
	}
}