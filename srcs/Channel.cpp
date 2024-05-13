/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 17:10:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"

Channel::Channel() :
	_name(""),
	_topic(""),
	_operators(),
	_users()
{
	fill(begin(_modes), end(_modes), false);
}

Channel::Channel(const Channel &copy) :
	_name(copy._name),
	_topic(copy._topic),
	_operators(copy._operators),
	_users(copy._users)
{
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = copy._modes[i];
}

Channel::~Channel() {}

Channel	&Channel::operator=(const Channel &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_topic = rhs._topic;
		_operators = rhs._operators; //operator overload di vector fa una deep copy
		_users = rhs._users;
		for (int i = 0; i < N_MODES; i++)
			_modes[i] = rhs._modes[i];
	}
	return *this;
}

string	Channel::getName() const
{
	return _name;
}

string	Channel::getTopic() const
{
	return _topic;
}

bool	Channel::getMode(const channel_modes &mode) const
{
	if (mode < 0 || mode >= N_MODES)
		throw NotExistingModeException();
	return _modes[mode];
}

void	Channel::setTopic(const string &new_topic)
{
	_topic = new_topic;
}

void	Channel::setMode(const ChannelOperator &op, const channel_modes &mode, const bool status)
{
	if (mode < 0 || mode >= N_MODES) //modalita' sbagliata
		throw NotExistingModeException();
	if (_operators[op.getNickname()] == NULL) //se l'user non e' un operatore di questo canale
		throw NotOperatorException();
	else
		_modes[mode] = status;
}

void	Channel::addUser(const User &user)
{
	_users[user.getNickname()] = &user;
}

void	Channel::addOperator(const ChannelOperator &op)
{
	if (_users[op.getNickname()] == NULL) //se l'operatore non e' un user di questo canale
		throw UserNotInChannelException();
	else
		_operators[op.getNickname()] = &op;
}

void	Channel::addRequest(const User &user)
{
	_requests[user.getNickname()] = &user;
}

void	Channel::removeUser(const User &user)
{
	_users.erase(user.getNickname());
}

