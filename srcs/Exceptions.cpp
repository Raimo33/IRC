/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 21:40:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <vector>

#include "irc/Exceptions.hpp"

using namespace std;
using namespace irc;

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

ProtocolErrorException::ProtocolErrorException(const uint16_t code, ...) :
	_code(code)
{
	va_list args;
	va_start(args, code);
	const char *param;

	while ((param = va_arg(args, const char *)))
		_params.push_back(string(param));
	va_end(args);
}

ProtocolErrorException::~ProtocolErrorException(void) throw() {}

const char *ProtocolErrorException::what(void) const throw()
{
	return "";
}

uint16_t ProtocolErrorException::getCode(void) const
{
	return _code;
}

const vector<string> &ProtocolErrorException::getParams(void) const
{
	return _params;
}
