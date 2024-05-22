/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 15:22:47 by craimond         ###   ########.fr       */
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

ProtocolErrorException::ProtocolErrorException(const uint16_t code, const vector<string> &params) :
	_code(code),
	_params(params) {}

const char *ProtocolErrorException::what(void) const throw()
{
	return "";
}



