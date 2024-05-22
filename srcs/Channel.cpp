/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 15:45:09 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/ChannelOperator.hpp"
#include "irc/utils.hpp"
#include "irc/Message.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"

using namespace std;
using namespace irc;

Channel::Channel(const string &name, ChannelOperator &op) :
	_name(name),
	_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
{
	if (!is_valid_channel_name(name))
		throw ProtocolErrorException(ERR_NOSUCHCHANNEL, vector<string>(1, name));
	_operators[op.getUsername()] = &op;
	_members[op.getUsername()] = &op;
	for (int i = 0; i < N_MODES; i++)
		_modes[i] = false;
}

Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
	_name(name),
	_key(key),
	_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
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
	if (new_key.empty() || new_key.length() > MAX_CHANNEL_KEY_LEN)
		throw InvalidKeyException();
	_key = new_key;
}

const string &Channel::getTopic(void) const
{
	return _topic;
}

void Channel::setTopic(const string &new_topic)
{
	if (new_topic.length() > MAX_CHANNEL_TOPIC_LEN)
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

const Client &Channel::getOperator(const string &nickname) const
{
	if (_operators.find(nickname) == _operators.end())
		throw UserNotMemberException();
	return *(_operators.at(nickname));
}

Client &Channel::addOperator(ChannelOperator *op)
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

const map<string, Client *> &Channel::getMembers(void) const
{
	return _members;
}

void Channel::setMembers(const map<string, Client *> &new_members)
{
	_members = new_members;
}

const Client &Channel::getMember(const string &nickname) const
{
	if (_members.find(nickname) == _members.end())
		throw UserNotMemberException();
	return *(_members.at(nickname));
}

void Channel::addMember(Client &user)
{
	if (_members.find(user.getUsername()) != _members.end())
	{
		vector<string> params(2);

		params.push_back(user.getUsername());
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	}
	if (_members.size() >= _member_limit)
		throw ChannelFullException();
	_members[user.getUsername()] = &user;
}

void Channel::removeMember(const Client &user)
{
	if (_members.find(user.getUsername()) == _members.end())
		throw UserNotMemberException();
	_members.erase(user.getUsername());
}

const map<string, Client *> &Channel::getPendingInvitations(void) const
{
	return _pending_invitations;
}

void Channel::setPendingInvitations(const map<string, Client *> &new_invitations)
{
	_pending_invitations = new_invitations;
}

const Client &Channel::getPendingInvitation(const string &nickname) const
{
	if (_pending_invitations.find(nickname) == _pending_invitations.end())
		throw UserNotMemberException();
	return *(_pending_invitations.at(nickname));
}

void Channel::addPendingInvitation(Client *user)
{
	if (_pending_invitations.find(user->getUsername()) != _pending_invitations.end())
	{
		vector<string> params(2);

		params.push_back(user->getUsername());
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	}
	_pending_invitations[user->getUsername()] = user;
}

void Channel::removePendingInvitation(const Client &user)
{
	if (_pending_invitations.find(user.getUsername()) == _pending_invitations.end())
	{
		vector<string> params(2);

		params.push_back(user.getUsername());
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	}
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

void Channel::promoteOperator(const string &nickname)
{
	if (_operators.find(nickname) != _operators.end())
		throw UserAlreadyOperatorException();
	if (_members.find(nickname) == _members.end())
		throw UserNotMemberException();
	_operators[nickname] = new ChannelOperator(getMember(nickname));
}

void Channel::demoteOperator(const string &nickname)
{
	if (_operators.find(nickname) == _operators.end())
		throw UserNotOperatorException();
	delete _operators[nickname];
	_operators.erase(nickname);
}

void	Channel::receiveMessage(const Message &msg) const
{
	for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
		EventHandler::sendBufferedString(*receiver->second, msg.getContent());
}

