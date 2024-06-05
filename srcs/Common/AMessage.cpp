/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:31:09 by egualand          #+#    #+#             */
/*   Updated: 2024/06/05 17:53:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"
#include "CommandMessage.hpp"
#include "ReplyMessage.hpp"
#include "common_constants.hpp"
#include "common_exceptions.hpp"
#include "system_calls.hpp"

using std::string;

AMessage::AMessage(void) {}

AMessage::AMessage(const AMessage &copy) :
	_prefix(copy._prefix),
	_params(copy._params) {}

AMessage::~AMessage(void) {}

AMessage &AMessage::operator=(const AMessage &copy)
{
	if (this == &copy)
		return (*this);
	_prefix = copy._prefix;
	_params = copy._params;
	return (*this);
}

const std::string			   &AMessage::getPrefix(void) const { return (_prefix); }
void							AMessage::setPrefix(const std::string &prefix) { _prefix = prefix; }
const std::vector<std::string> &AMessage::getParams(void) const { return (_params); }
void							AMessage::setParams(const std::vector<std::string> &params) { _params = params; }

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

bool AMessage::isReply(void) const
{
	return (dynamic_cast<const ReplyMessage *>(this) != NULL);
}

bool AMessage::isCommand(void) const
{
	return (dynamic_cast<const CommandMessage *>(this) != NULL);
}

const std::string AMessage::getRaw(void) const
{
	string first_part, second_part;
	unwrapMessage(first_part, second_part);
	return (first_part + second_part);
}

void AMessage::getDelivered(const int socket) const
{
	string first_part, second_part;
	unwrapMessage(first_part, second_part);

	size_t				   first_part_len = first_part.size();
	const size_t		   block_size = BUFFER_SIZE - first_part_len - 2;
	string::const_iterator it = second_part.begin();
	string::const_iterator end = second_part.end();
	size_t				   remaining;
	size_t				   second_part_len;
	size_t				   combined_len;

	while (it != end)
	{
		remaining = std::distance(it, end);
		second_part_len = std::min(block_size, remaining);
		combined_len = first_part_len + second_part_len;

		char *to_send = new char[combined_len + 2];
		std::copy(first_part.begin(), first_part.end(), to_send);
		std::copy(it, it + second_part_len, to_send + first_part_len);
		to_send[combined_len] = '\r';
		to_send[combined_len + 1] = '\n';

		it += second_part_len;
		send_p(socket, to_send, combined_len + 2, 0);
		delete[] to_send;
	}
}