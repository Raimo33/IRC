/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 23:51:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Client.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ReplyMessage.hpp"
#include "server_utils.hpp"
#include "server_exceptions.hpp"
#include "server_constants.hpp"

#include <sstream>

using std::map;
using std::set;
using std::string;
using std::stringstream;
using std::vector;

Channel::Channel(Logger &logger, const string &name, Client &op, const string &key) : 
	_name(name),
	_member_limit(-1),
	_modes(initModes()),
	_logger(logger)
{
	checkName(name);
	_operators.insert(&op);
	op.joinChannel(*this);
	// setMode('o', true, op.getNickname());
	// setMode('t', true);
	if (!key.empty())
		setMode('k', true, key);
	ostringstream oss;
	oss << "Channel created: " << name;
	_logger.logEvent(oss.str());
}

Channel::Channel(const Channel &copy) : 
	_name(copy._name),
	_key(copy._key),
	_topic(copy._topic),
	_member_limit(copy._member_limit),
	_members(copy._members),
	_operators(copy._operators),
	_pending_invitations(copy._pending_invitations),
	_modes(copy._modes),
	_logger(copy._logger)
{
	ostringstream oss;
	oss << "Channel created: " << _name;
	_logger.logEvent(oss.str());
}

Channel::~Channel(void)
{
	ostringstream oss;
	oss << "Channel deleted: " << _name;
	_logger.logEvent(oss.str());
}

const string &Channel::getName(void) const
{
	return _name;
}

void Channel::setName(const string &new_name)
{
	checkName(new_name);
	ostringstream oss;
	oss << "Channel " << _name << " renamed to " << new_name;
	_logger.logEvent(oss.str());
	_name = new_name;
}

const string &Channel::getKey(void) const
{
	return _key;
}

void Channel::setKey(const string &new_key)
{
	if (!is_valid_channel_key(new_key))
		throw ActionFailedException(ERR_BADCHANNELKEY, _name.c_str(), (new_key + " is not a valid channel key").c_str(), NULL);
	_key = new_key;
	ostringstream oss;
	oss << "Channel " << _name << " key set to " << new_key;
	_logger.logEvent(oss.str());
}

const string &Channel::getTopic(void) const
{
	return _topic;
}

void Channel::setTopic(const string &new_topic)
{
	if (new_topic.length() > MAX_CHANNEL_TOPIC_LEN)
		throw ActionFailedException(RPL_NOTOPIC, _name.c_str(), (new_topic + " is too long").c_str(), NULL);
	_topic = new_topic;
	ostringstream oss;
	oss << "Channel " << _name << " topic set to " << new_topic;
	_logger.logEvent(oss.str());
}

uint32_t Channel::getMemberLimit(void) const
{
	return _member_limit;
}

void Channel::setMemberLimit(const uint32_t new_limit)
{
	_member_limit = new_limit;
}

const map<string, Client *> &Channel::getMembers(void) const
{
	return _members;
}

void Channel::setMembers(const map<string, Client *> &new_members)
{
	_members = new_members;
}

Client &Channel::getMember(const string &nickname) const
{
	if (!isMember(nickname))
		throw ActionFailedException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str(), g_default_replies_map.at(ERR_USERNOTINCHANNEL), NULL);
	return *(_members.at(nickname));
}

void Channel::addMember(Client &user)
{
	const string &nickname = user.getNickname();

	if (_modes['i'] && _pending_invitations.find(&user) == _pending_invitations.end())
		throw ActionFailedException(ERR_INVITEONLYCHAN, _name.c_str(), ("You must be invited to join " + _name).c_str(), NULL);
	if (isMember(user))
		throw ActionFailedException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str(), (nickname + " is already on " + _name).c_str(), NULL);
	if (_members.size() >= _member_limit)
		throw ActionFailedException(ERR_CHANNELISFULL, _name.c_str(), g_default_replies_map.at(ERR_CHANNELISFULL), NULL);
	_members[nickname] = &user;
	ostringstream oss;
	oss << "Channel " << _name << ", member added: " << nickname;
	_logger.logEvent(oss.str());
}

void Channel::removeMember(const string &nickname)
{
	Client &user = getMember(nickname);
	if (isOperator(user))
		_operators.erase(&user);
	_members.erase(nickname);
	ostringstream oss;
	oss << "Channel " << _name << ", member removed: " << nickname;
	_logger.logEvent(oss.str());
	if (_members.empty())
		delete this;
}

const set<const Client *> &Channel::getOperators(void) const
{
	return _operators;
}

void Channel::setOperators(const set<const Client *> &new_operators)
{
	_operators = new_operators;
}

void Channel::addOperator(const string &nickname)
{
	const Client &user = getMember(nickname);
	if (isOperator(user))
		throw ActionFailedException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str(), (nickname + " is already an operator of " + _name).c_str(), NULL);
	_operators.insert(&user);

	ostringstream oss;
	oss << "Channel " << _name << ", operator added: " << nickname;
	_logger.logEvent(oss.str());
}

void Channel::removeOperator(const string &nickname)
{
	const Client &user = getMember(nickname);
	if (!isOperator(user))
		throw ActionFailedException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str(), (nickname + " is not an operator of " + _name).c_str(), NULL);
	_operators.erase(&user);

	ostringstream oss;
	oss << "Channel " << _name << ", operator removed: " << nickname;
	_logger.logEvent(oss.str());
}

const set<const Client *> &Channel::getPendingInvitations(void) const
{
	return _pending_invitations;
}

void Channel::setPendingInvitations(const set<const Client *> &new_invitations)
{
	_pending_invitations = new_invitations;
}

void Channel::addPendingInvitation(Client &user)
{
	const string &nickname = user.getNickname();

	if (isMember(user))
		throw ActionFailedException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str(), g_default_replies_map.at(ERR_USERONCHANNEL), NULL);
	if (_pending_invitations.find(&user) != _pending_invitations.end())
		throw ActionFailedException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str(), (nickname + " is already invited to " + _name).c_str(), NULL);
	_pending_invitations.insert(&user);
	ostringstream oss;
	oss << "Channel " << _name << ", invitation sent to: " << nickname;
	_logger.logEvent(oss.str());
}

void Channel::removePendingInvitation(Client &user)
{
	const string &nickname = user.getNickname();

	if (_pending_invitations.find(&user) == _pending_invitations.end())
		throw ActionFailedException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str(), (nickname + " was not invited to " + _name).c_str(), NULL);
	_pending_invitations.erase(&user);
	ostringstream oss;
	oss << "Channel " << _name << ", invitation to " << nickname << " removed";
	_logger.logEvent(oss.str());
}

const map<char, bool> &Channel::getModes(void) const
{
	return _modes;
}

void Channel::setModes(const map<char, bool> &modes, const vector<string> &params, const Client *setter)
{
	uint32_t i = 0;

	for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (channel_mode_requires_param(it->first, it->second))
		{
			if (i >= params.size())
				throw ActionFailedException(ERR_NEEDMOREPARAMS, _name.c_str(), ("Missing parameter for mode " + string(1, it->first)).c_str(), NULL);
			setMode(it->first, it->second, params[i++], setter);
		}
		else
			setMode(it->first, it->second, "", setter);
	}
}

bool Channel::getMode(const char mode) const
{
	const map<char, bool>::const_iterator it = _modes.find(mode);
	if (it == _modes.end())
		throw ActionFailedException(ERR_UNKNOWNMODE, _name.c_str(), (string(1, mode) + " is not a valid channel mode").c_str(), NULL);
	return it->second;
}

void Channel::setMode(const char mode, const bool status, const string &param, const Client *setter)
{
	if (channel_mode_requires_param(mode, status) && param.empty())
		throw InternalErrorException("Channel::setMode: missing parameter for mode");
	if (mode == 'k')
	{
		if (status)
			setKey(param);
		else
			_key = "";
	}
	else if (mode == 'l')
	{
		if (status)
		{
			stringstream ss(param);
			uint32_t new_limit;

			if (!(ss >> new_limit) || !ss.eof())
				throw ActionFailedException(ERR_NEEDMOREPARAMS, _name.c_str(), "Invalid parameter for mode 'l'");
			setMemberLimit(new_limit);
		}
		else
			_member_limit = -1;
	}
	else if (mode == 'o')
		status ? addOperator(param) : removeOperator(param);

	if (_modes.at(mode) != status)
	{
		ostringstream oss;
		oss << "Channel " << _name << ", mode " << mode << " is now " << (status ? "on" : "off");
		_logger.logEvent(oss.str());
	}
	_modes[mode] = status;

	string prefix = setter ? setter->getNickname() : SERVER_NAME;
	string mode_str = (status ? "+" : "-") + string(1, mode);
	const CommandMessage mode_change(prefix, MODE, _name.c_str(), mode_str.c_str(), param.c_str(), NULL);
	receiveMessage(mode_change);
}

void Channel::receiveMessage(const AMessage &msg, const Client *sender) const
{
	for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
		if (!sender || receiver->second != sender)
			receiver->second->receiveMessage(&msg);
}

bool Channel::isOperator(const Client &user) const
{
	return _operators.find(&user) != _operators.end();
}

bool Channel::isMember(const string &nickname) const
{
	return _members.find(nickname) != _members.end();
}

bool Channel::isMember(const Client &user) const
{
	return isMember(user.getNickname());
}

string Channel::getMembersString(void) const
{
	string members_str;

	members_str.reserve(_members.size() * 10);
	for (set<const Client *>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
		members_str += "@" + (*it)->getNickname() + " ";
	for (map<string, Client *>::const_iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (!isOperator(*it->second))
			members_str += it->first + " ";
	}
	members_str.erase(members_str.length() - 1);
	return members_str;
}

const map<char, bool> Channel::initModes(void) const
{
	map<char, bool> modes;

	modes['i'] = false;
	modes['t'] = false;
	modes['k'] = false;
	modes['o'] = false;
	modes['l'] = false;
	return modes;
}

void Channel::checkName(const string &name) const
{
	if (!is_valid_channel_name(name))
		throw ActionFailedException(ERR_NOSUCHCHANNEL, name.c_str(), (name + " is not a valid channel name").c_str(), NULL);
}

void Channel::checkKey(const string &key) const
{
	if (key != _key)
		throw ActionFailedException(ERR_BADCHANNELKEY, _name.c_str(), ("wrong key for " + _name).c_str(), NULL);
}
