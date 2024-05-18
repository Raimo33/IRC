/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 11:28:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"

#include "headers/User.hpp"
#include "headers/ChannelOperator.hpp"
#include "headers/utils.hpp"

Channel::Channel(const string &name, ChannelOperator &op) :
	_name(name),
	_key(""),
	_topic(""),
	_operators(),
	_members(),
	_requests()
{
	if (_name.empty() || is_channel_prefix(_name[0]) == false)
		throw InvalidNameException();
	_operators[op.getNickname()] = &op;
	_members[op.getNickname()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
}

Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
	_key(key),
	_topic(""),
	_operators(),
	_members(),
	_requests()
{
	if (_name.empty() || is_channel_prefix(_name[0]) == false)
		throw InvalidNameException();
	_operators[op.getNickname()] = &op;
	_members[op.getNickname()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
	_modes[MODE_K] = true;
}

Channel::Channel(const Channel &copy) :
	_name(copy._name),
	_topic(copy._topic),
	_operators(copy._operators),
	_members(copy._members),
	_requests(copy._requests)
{
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = copy._modes[i];
}

Channel::~Channel(void) {}

Channel	&Channel::operator=(const Channel &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_topic = rhs._topic;
		_operators = rhs._operators; //operator overload di vector fa una deep copy
		_members = rhs._members;
		for (int i = 0; i < N_MODES; i++)
			_modes[i] = rhs._modes[i];
	}
	return *this;
}

string	Channel::getName(void) const
{
	return _name;
}

string	Channel::getKey(void) const
{
	return _key;
}

string	Channel::getTopic(void) const
{
	return _topic;
}

bool	Channel::getMode(const t_channel_modes &mode) const
{
	if (mode < 0 || mode >= N_MODES)
		throw NotExistingModeException();
	return _modes[mode];
}

User	&Channel::getOperator(const string &nickname) const
{
	if (_operators.at(nickname) == NULL)
		throw OperatorNotFoundException();
	return *_operators.at(nickname);
}

User	&Channel::getMember(const string &nickname) const
{
	if (_members.at(nickname) == NULL)
		throw MemberNotFoundException();
	return *_members.at(nickname);
}

User	&Channel::getRequest(const string &nickname) const
{
	if (_requests.at(nickname) == NULL)
		throw RequestNotFoundException();
	return *_requests.at(nickname);
}

map<string, User *>	Channel::getUsers(void) const
{
	return _members;
}

uint32_t	Channel::getMembersCount(void) const
{
	return _members.size();
}

void	Channel::setKey(const string &new_key)
{
	_key = new_key;
}

void	Channel::setTopic(const string &new_topic)
{
	_topic = new_topic;
}

void	Channel::setMode(const t_channel_modes &mode, const bool status)
{
	if (mode < 0 || mode >= N_MODES) //modalita' sbagliata
		throw NotExistingModeException();
	_modes[mode] = status;
	//TODO implementare le effettive modifiche alle modalita' del canale. esempio: key deve cambiare la chiave del canale
}

bool	Channel::handleJoinRequest(User &user)
{
	const string nickname = user.getNickname();
	
	if (_modes[MODE_I])
	{
		if (!_pending_invitations.at(nickname))
			_requests[nickname] = &user;
		else
		{
			_requests.erase(nickname);
			_members[nickname] = &user;
		}
	}
	else
		_members[nickname] = &user;
	return (_members[nickname] != NULL);
}

void	Channel::addUser(User &user)
{
	_members[user.getNickname()] = &user;
}

void	Channel::addOperator(ChannelOperator &op)
{
	if (_members[op.getNickname()] == NULL) //se l'operatore non e' un user di questo canale
		throw UserNotInChannelException();
	else
		_operators[op.getNickname()] = &op;
}

void	Channel::addRequest(User &user)
{ 
	_requests[user.getNickname()] = &user;
}

void	Channel::removeUser(const User &user)
{
	_members.erase(user.getNickname());
}

const char *Channel::UserNotInChannelException::what() const throw()
{
	return "User not in channel";
}

const char *Channel::NotOperatorException::what() const throw()
{
	return "User not an operator";
}

const char *Channel::NotExistingModeException::what() const throw()
{
	return "Mode does not exist";
}

const char *Channel::InvalidCredentialsException::what() const throw()
{
	return "Invalid credentials";
}

const char *Channel::MemberNotFoundException::what() const throw()
{
	return "Member not found";
}

const char *Channel::OperatorNotFoundException::what() const throw()
{
	return "Operator not found";
}

const char *Channel::RequestNotFoundException::what() const throw()
{
	return "Request not found";
}

const char *Channel::InvalidNameException::what() const throw()
{
	return "Invalid channel name";
}

