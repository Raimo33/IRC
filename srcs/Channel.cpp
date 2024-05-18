/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 16:31:34 by egualand         ###   ########.fr       */
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
	_member_limit(DEFAULT_MEMBER_LIMIT),
	_operators(),
	_members(),
	_requests()
{
	if (_name.empty() || !is_channel_prefix(_name[0]) || _name.length() > MAX_CHANNEL_NAME_LEN)
		throw InvalidNameException();
	_operators[op.getNickname()] = &op;
	_members[op.getNickname()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
}

Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
	_key(key),
	_topic(""),
	_member_limit(DEFAULT_MEMBER_LIMIT),
	_operators(),
	_members(),
	_requests()
{
	setName(name);
	if (_name.empty() || !is_channel_prefix(_name[0]) || _name.length() > MAX_CHANNEL_NAME_LEN)
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

const string Channel::getName(void) const
{
	return _name;
}

void Channel::setName(const string &new_name)
{
	if (new_name.empty() || !is_channel_prefix(new_name[0]) || new_name.length() > MAX_CHANNEL_NAME_LEN)
		throw InvalidNameException();
	_name = new_name;
}

const string Channel::getKey(void) const
{
	return _key;
}

void Channel::setKey(const string &new_key)
{
	if (new_key.empty() || new_key.length() > MAX_KEY_LEN)
		throw InvalidKeyException();
	_key = new_key;
}

const string Channel::getTopic(void) const
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

const map<string, ChannelOperator *> Channel::getOperators(void) const
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
	if (_operators.find(op->getNickname()) != _operators.end())
		throw UserAlreadyOperatorException();
	_operators[op->getNickname()] = op;
	return *op;
}

void Channel::removeOperator(const ChannelOperator &op)
{
	if (_operators.find(op.getNickname()) == _operators.end())
		throw UserNotMemberException();
	_operators.erase(op.getNickname());
}

const map<string, User *> Channel::getMembers(void) const
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
	if (_members.find(user->getNickname()) != _members.end())
		throw UserAlreadyMemberException();
	_members[user->getNickname()] = user;
}

void Channel::removeMember(const User &user)
{
	if (_members.find(user.getNickname()) == _members.end())
		throw UserNotMemberException();
	_members.erase(user.getNickname());
}

const map<string, User *> Channel::getRequests(void) const
{
	return _requests;
}

void Channel::setRequests(const map<string, User *> &new_requests)
{
	_requests = new_requests;
}

const User &Channel::getRequest(const string &username) const
{
	if (_requests.find(username) == _requests.end())
		throw UserNotMemberException();
	return *(_requests.at(username));
}

void Channel::addRequest(User *user)
{
	if (_requests.find(user->getNickname()) != _requests.end())
		throw UserAlreadyMemberException();
	_requests[user->getNickname()] = user;
}

void Channel::removeRequest(const User &user)
{
	if (_requests.find(user.getNickname()) == _requests.end())
		throw UserNotMemberException();
	_requests.erase(user.getNickname());
}

const map<string, User *> Channel::getPendingInvitations(void) const
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
	if (_pending_invitations.find(user->getNickname()) != _pending_invitations.end())
		throw UserAlreadyMemberException();
	_pending_invitations[user->getNickname()] = user;
}

void Channel::removePendingInvitation(const User &user)
{
	if (_pending_invitations.find(user.getNickname()) == _pending_invitations.end())
		throw UserNotMemberException();
	_pending_invitations.erase(user.getNickname());
}

bool *Channel::getModes(void) const
{
	return (bool *)_modes;
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

bool Channel::handleJoinRequest(User &user)
{
	if (_requests.find(user.getNickname()) == _requests.end())
		return false;
	_members[user.getNickname()] = &user;
	_requests.erase(user.getNickname());
	return true;
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

