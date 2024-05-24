/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 16:16:31 by craimond         ###   ########.fr       */
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

	ProtocolErrorException::ProtocolErrorException(const struct s_replyContent &content) : //potremmo anche fare il throw della struct direttamente
		_content(content) {}

	ProtocolErrorException::~ProtocolErrorException(void) throw() {}

	const struct s_replyContent	&ProtocolErrorException::getContent(void) const
	{
		return _content;
	}
}