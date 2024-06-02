/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:31:09 by egualand          #+#    #+#             */
/*   Updated: 2024/06/02 17:20:24 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"
#include "system_calls.hpp"
#include "common_exceptions.hpp"
#include "common_constants.hpp"

using std::string;

AMessage::AMessage(void) {}

AMessage::AMessage(const AMessage &copy) : _prefix(copy._prefix),
										   _params(copy._params) {}

AMessage::~AMessage(void) {}

AMessage &AMessage::operator=(const AMessage &copy)
{
	if (this != &copy)
	{
		_prefix = copy._prefix;
		_params = copy._params;
	}
	return (*this);
}

const std::string &AMessage::getPrefix(void) const { return (_prefix); }
void AMessage::setPrefix(const std::string &prefix) { _prefix = prefix; }
const std::vector<std::string> &AMessage::getParams(void) const { return (_params); }
void AMessage::setParams(const std::vector<std::string> &params) { _params = params; }

void AMessage::setParam(const std::string &param, int32_t index)
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

void AMessage::deliver(const int socket) const
{
	string first_part, second_part, to_send;
	uint16_t block_size, send_length;

	unwrapMessage(first_part, second_part);
	block_size = BUFFER_SIZE - first_part.size() - 2; // 2 per \r\n
	while (!second_part.empty())
	{
		send_length = std::min(static_cast<size_t>(block_size), second_part.size());
		to_send = first_part + second_part.substr(0, send_length) + "\r\n";
		second_part = second_part.substr(send_length); // Update second_part correctly
		send_p(socket, to_send.c_str(), to_send.length(), 0);
	}
}
