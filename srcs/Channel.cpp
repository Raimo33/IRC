/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 16:18:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Channel.hpp"

#include "headers/User.hpp"
#include "headers/ChannelOperator.hpp"
#include "headers/utils.hpp"

Channel::Channel(const string &name, ChannelOperator &op) :
	_name(name),
	_member_limit(DEFAULT_MEMBER_LIMIT)
{
	if (!is_valid_channel_name(name))
		throw InvalidNameException();
	_operators[op.getUsername()] = &op;
	_members[op.getUsername()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
}

Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
	_name(name),
	_key(key),
	_member_limit(DEFAULT_MEMBER_LIMIT)
{
	if (!is_valid_channel_name(name))
		throw InvalidNameException();
	_operators[op.getUsername()] = &op;
	_members[op.getUsername()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
	_modes[MODE_K] = true;
}

Channel::Channel(const Channel &copy) :
	_name(copy._name),
	_topic(copy._topic),
	_operators(copy._operators),
	_members(copy._members),
	_pending_invitations(copy._pending_invitations)
{
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = copy._modes[i];
}

Channel::~Channel(void) {}

const string &Channel::getName(void) const
{
	return _name;
}

void Channel::setName(const string &new_name)
{
	if (!is_valid_channel_name(new_name))
		throw InvalidNameException();
	_name = new_name;
}

const string &Channel::getKey(void) const
{
	return _key;
}

void Channel::setKey(const string &new_key)
{
	if (new_key.empty() || new_key.length() > MAX_KEY_LEN)
		throw InvalidKeyException();
	_key = new_key;
}

const string &Channel::getTopic(void) const
{
	return _topic;
}

void Channel::setTopic(const string &new_topic)
{
	if (new_topic.length() > MAX_TOPIC_LEN)
		throw InvalidTopicException();
	_topic = new_topic;
}

uint32_t Channel::getMemberLimit(void) const
{
	return _member_limit;
}

void Channel::setMemberLimit(const uint32_t new_limit)
{
	_member_limit = new_limit;
}

const map<string, ChannelOperator *> &Channel::getOperators(void) const
{
	return _operators;
}

void Channel::setOperators(const map<string, ChannelOperator *> &new_operators)
{
	_operators = new_operators;
}

const User &Channel::getOperator(const string &username) const
{
	if (_operators.find(username) == _operators.end())
		throw UserNotMemberException();
	return *(_operators.at(username));
}

User &Channel::addOperator(ChannelOperator *op)
{
	if (_operators.find(op->getUsername()) != _operators.end())
		throw UserAlreadyOperatorException();
	_operators[op->getUsername()] = op;
	return *op;
}

void Channel::removeOperator(const ChannelOperator &op)
{
	if (_operators.find(op.getUsername()) == _operators.end())
		throw UserNotMemberException();
	_operators.erase(op.getUsername());
}

const map<string, User *> &Channel::getMembers(void) const
{
	return _members;
}

void Channel::setMembers(const map<string, User *> &new_members)
{
	_members = new_members;
}

const User &Channel::getMember(const string &username) const
{
	if (_members.find(username) == _members.end())
		throw UserNotMemberException();
	return *(_members.at(username));
}

void Channel::addMember(User *user)
{
	if (_members.find(user->getUsername()) != _members.end())
		throw UserAlreadyMemberException();
	_members[user->getUsername()] = user;
}

void Channel::removeMember(const User &user)
{
	if (_members.find(user.getUsername()) == _members.end())
		throw UserNotMemberException();
	_members.erase(user.getUsername());
}

const map<string, User *> &Channel::getPendingInvitations(void) const
{
	return _pending_invitations;
}

void Channel::setPendingInvitations(const map<string, User *> &new_invitations)
{
	_pending_invitations = new_invitations;
}

const User &Channel::getPendingInvitation(const string &username) const
{
	if (_pending_invitations.find(username) == _pending_invitations.end())
		throw UserNotMemberException();
	return *(_pending_invitations.at(username));
}

void Channel::addPendingInvitation(User *user)
{
	if (_pending_invitations.find(user->getUsername()) != _pending_invitations.end())
		throw UserAlreadyMemberException();
	_pending_invitations[user->getUsername()] = user;
}

void Channel::removePendingInvitation(const User &user)
{
	if (_pending_invitations.find(user.getUsername()) == _pending_invitations.end())
		throw UserNotMemberException();
	_pending_invitations.erase(user.getUsername());
}

const bool *Channel::getModes(void) const
{
	return _modes;
}

void Channel::setModes(const bool new_modes[N_MODES])
{
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = new_modes[i];
}

bool Channel::getMode(const t_channel_modes &mode) const
{
	return _modes[mode];
}

void Channel::setMode(const t_channel_modes &mode, const bool value)
{
	_modes[mode] = value;
}

void Channel::promoteOperator(const string &username)
{
	if (_operators.find(username) != _operators.end())
		throw UserAlreadyOperatorException();
	if (_members.find(username) == _members.end())
		throw UserNotMemberException();
	_operators[username] = new ChannelOperator(getMember(username));
}

void Channel::demoteOperator(const string &username)
{
	if (_operators.find(username) == _operators.end())
		throw UserNotOperatorException();
	delete _operators[username];
	_operators.erase(username);
}

const char	*Channel::InvalidNameException::what() const throw()
{
	return "Invalid channel name";
}

const char *Channel::InvalidTopicException::what() const throw()
{
	return "Invalid channel topic";
}

const char *Channel::InvalidKeyException::what() const throw()
{
	return "Invalid channel key";
}

const char *Channel::UserAlreadyMemberException::what() const throw()
{
	return "User is already a member of this channel";
}

const char *Channel::UserAlreadyOperatorException::what() const throw()
{
	return "User is already an operator of this channel";
}

const char *Channel::UserNotMemberException::what() const throw()
{
	return "User is not a member of this channel";
}

const char *Channel::UnknownModeException::what() const throw()
{
	return "Unknown channel mode";
}

