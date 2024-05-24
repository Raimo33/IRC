/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 12:43:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <map>
#include <vector>
#include <string>

#include "irc/Exceptions.hpp"
#include "irc/Constants.hpp"
#include "irc/ReplyCodes.hpp"

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

	ProtocolErrorException::ProtocolErrorException(const string &custom_msg, const uint32_t code, ...)
	{
		string	param;
		va_list	args;

		va_start(args, code);
		_content.prefix = SERVER_NAME;
		_content.code = code;
		param = string(va_arg(args, const char *));
		while (!param.empty())
		{
			_content.params.push_back(param);
			param = string(va_arg(args, const char *));
		}
		va_end(args);

		if (custom_msg.empty())
		{
			map<uint16_t, string>::const_iterator it = reply_codes.find(code);
			if (it == reply_codes.end())
				throw InternalErrorException("ProtocolErrorException::ProtocolErrorException: Unknown reply code");
			_content.text = it->second;
		}
		else
			_content.text = custom_msg;
	}

	ProtocolErrorException::~ProtocolErrorException(void) throw() {}

	const struct s_replyContent	&ProtocolErrorException::getContent(void) const
	{
		return _content;
	}
}