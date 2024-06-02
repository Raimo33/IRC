/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:31:09 by egualand          #+#    #+#             */
/*   Updated: 2024/06/02 15:33:41 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"

AMessage::AMessage(void) {}

AMessage::AMessage(const std::string &raw_input)
{
	parse(raw_input);
}

AMessage::AMessage(const AMessage &copy) :
	_prefix(copy._prefix),
	_params(copy._params) {}

AMessage::~AMessage(void) {}

AMessage	&AMessage::operator=(const AMessage &copy)
{
	if (this != &copy)
	{
		_prefix = copy._prefix;
		_params = copy._params;
	}
	return (*this);
}

const std::string				&AMessage::getPrefix(void) const { return (_prefix); }
void							AMessage::setPrefix(const std::string &prefix) { _prefix = prefix; }
const std::vector<std::string>	&AMessage::getParams(void) const { return (_params); }
void							AMessage::setParams(const std::vector<std::string> &params) { _params = params; }

void							AMessage::setParam(const std::string &param, int32_t index)
{
	if (_params.empty())
	{
		_params.push_back(param);
		return;
	}
	if (index < 0)
		index += _params.size();

	if (index < 0 || index > static_cast<int32_t>(_params.size()))
		throw InternalErrorException("Message::setParam: Index out of range");
	_params.insert(_params.begin() + index, param);
}

