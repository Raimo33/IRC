/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/29 15:58:52 by craimond         ###   ########.fr       */
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
#include "irc/ReplyCodes.hpp"
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

ProtocolErrorException::ProtocolErrorException(const enum e_replyCodes code, const vector<string> &params, const string &custom_msg) :
	_content(EventHandler::buildReplyMessage(code, params, custom_msg)) {}

ProtocolErrorException::ProtocolErrorException(const enum e_replyCodes code, const string &param, const string &custom_msg) :
	_content(EventHandler::buildReplyMessage(code, param, custom_msg)) {}

ProtocolErrorException::~ProtocolErrorException(void) throw() {}

const char *ProtocolErrorException::what(void) const throw()
{
	return _content.text.c_str();
}

const struct s_replyMessage	&ProtocolErrorException::getContent(void) const
{
	return _content;
}