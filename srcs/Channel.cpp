/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 15:55:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/Client.hpp"
#include "irc/utils.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Constants.hpp"
#include "irc/Messages.hpp"
#include "irc/Logger.hpp"

#include <sstream>

using std::string;
using std::map;
using std::set;
using std::vector;
using std::stringstream;

Channel::Channel(Logger &logger, const string &name, Client &op, const string &key) :
	_name(name),
	_key(key),
	_topic(""),
	_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT),
	_members(map<string, Client *>()),
	_operators(set<Client *>()),
	_pending_invitations(set<Client *>()),
	_modes(256, false),
	_logger(logger)
{
	checkName(name);
	op.joinChannel(*this);
	addOperator(op);
	if (!key.empty())
		_modes['k'] = true;
	_logger.logEvent("Channel created: " + name);
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
	_logger.logEvent("Channel created: " + _name);
}

Channel::~Channel(void)
{
	_logger.logEvent("Channel deleted: " + _name);
}

const string &Channel::getName(void) const
{
	return _name;
}

void Channel::setName(const string &new_name)
{
	checkName(new_name);
	_logger.logEvent("Channel " + _name + " renamed to " + new_name);
	_name = new_name;
}

const string &Channel::getKey(void) const
{
	return _key;
}

void Channel::setKey(const string &new_key)
{
	checkKey(new_key);
	_key = new_key;
	_logger.logEvent("Channel " + _name + " key set to " + new_key);
}

const string &Channel::getTopic(void) const
{
	return _topic;
}

void Channel::setTopic(const string &new_topic)
{
	if (new_topic.length() > MAX_CHANNEL_TOPIC_LEN)
		throw ProtocolErrorException(RPL_NOTOPIC, _name, new_topic + " is too long");	
	_topic = new_topic;
	_logger.logEvent("Channel " + _name + " topic set to " + new_topic);
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

const Client &Channel::getMember(const string &nickname) const
{
	if (_members.find(nickname) == _members.end())
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);
	}
	return *(_members.at(nickname));
}

void Channel::addMember(Client &user)
{
	const string &nickname = user.getNickname();

	if (_modes['i'] && _pending_invitations.find(&user) == _pending_invitations.end())
		throw ProtocolErrorException(ERR_INVITEONLYCHAN, _name);
	if (_members.find(nickname) != _members.end())
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	}
	if (_members.size() >= _member_limit)
		throw ProtocolErrorException(ERR_CHANNELISFULL, _name);
	_members[nickname] = &user;
	_logger.logEvent("Channel " + _name + ", member added: " + nickname);
}

void Channel::removeMember(const Client &user)
{
	const string &nickname = user.getNickname();
	
	if (_members.find(nickname) == _members.end())
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);	
	}
	_members.erase(nickname);
	_logger.logEvent("Channel " + _name + ", member removed: " + nickname);
}

const set<Client *> &Channel::getOperators(void) const
{
	return _operators;
}

void Channel::setOperators(const set<Client *> &new_operators)
{
	_operators = new_operators;
}

void Channel::addOperator(Client &op)
{
	string	nickname = op.getNickname();

	if (isOperator(op) == true)
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERONCHANNEL, params, nickname + " is already an operator of " + _name);
	}
	_operators.insert(&op);

	_logger.logEvent("Channel " + _name + ", operator added: " + nickname);
	const struct s_replyMessage youreoper = EventHandler::buildReplyContent(RPL_YOUREOPER, nickname, "You are now an operator of " + _name);
	op.receiveMessage(youreoper);
}

void Channel::removeOperator(Client &op)
{
	string	nickname = op.getNickname();

	if (!isOperator(op))
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params, nickname + " is not an operator of " + _name);	
	}
	_operators.erase(&op);

	_logger.logEvent("Channel " + _name + ", operator removed: " + nickname);
	const struct s_replyMessage notoperanymore = EventHandler::buildReplyContent(RPL_NOTOPERANYMORE);
	op.receiveMessage(notoperanymore);
}

const set<Client *> &Channel::getPendingInvitations(void) const
{
	return _pending_invitations;
}

void Channel::setPendingInvitations(const set<Client *> &new_invitations)
{
	_pending_invitations = new_invitations;
}

void Channel::addPendingInvitation(Client &user)
{
	const string &nickname = user.getNickname();

	vector<string> params;
	params.push_back(nickname);
	params.push_back(_name);
	if (_members.find(nickname) != _members.end())
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	if (_pending_invitations.find(&user) != _pending_invitations.end())
		throw ProtocolErrorException(ERR_USERONCHANNEL, params, nickname + " is already invited to " + _name);
	_pending_invitations.insert(&user);
	_logger.logEvent("Channel " + _name + ", invitation sent to: " + nickname);
}

void Channel::removePendingInvitation(Client &user)
{
	const string &nickname = user.getNickname();

	if (_pending_invitations.find(&user) == _pending_invitations.end())
	{
		vector<string> params;
		params.push_back(nickname);
		params.push_back(_name);
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params, nickname + " was not invited to " + _name);
	}
	_pending_invitations.erase(&user);
	_logger.logEvent("Channel " + _name + ", invitation to " + nickname + " removed");
}

const vector<bool> &Channel::getModes(void) const
{
	return _modes;
}

void Channel::setModes(const vector<bool> &modes, const vector<string> &params)
{
	uint16_t	j = 0;

	for (uint8_t i = 0; i < (sizeof(_modes) / sizeof(_modes[0])); i++)
	{
		if (channel_mode_requires_param(i))
		{
			if (params.size() - 1 < j)
				throw InternalErrorException("Channel::setModes: missing parameter for mode");
			setMode(i, modes[i], params[j++]);
		}
		else
			setMode(i, modes[i]);
	}
}

bool Channel::getMode(const char mode) const
{
	return _modes[mode];
}

void Channel::setMode(const char mode, const bool status, const string &param)
{
	if (channel_mode_requires_param(mode) && param.empty())
		throw InternalErrorException("Channel::setMode: missing parameter for mode");
	_modes[mode] = status;
	if (mode == 'k')
		setKey(param);
	else if (mode == 'l')
	{
		stringstream	ss(param);
		uint32_t		new_limit;

		if (!(ss >> new_limit) || !ss.eof())
			throw ProtocolErrorException(ERR_NEEDMOREPARAMS, _name, "Invalid parameter for mode 'l'");
		setMemberLimit(new_limit);
	}
	else if (mode == 'o')
	{
		Client	op(getMember(param));

		status ? addOperator(op) : removeOperator(op);
	}
	else
		_logger.logEvent("Channel " + _name + ", mode " + mode + " is now " + (status ? "on" : "off"));

	string mode_str = (status ? "+" : "-") + mode;
	vector<string> params;
	params.push_back(_name);
	params.push_back(mode_str);
	params.push_back(param);
	const struct s_commandMessage mode_change = EventHandler::buildCommandContent(SERVER_NAME, MODE, params);
	receiveMessage(mode_change);
}

void	Channel::receiveMessage(const struct s_commandMessage &msg) const
{
	for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
	{
		if (receiver->first != msg.prefix)
			receiver->second->receiveMessage(msg);
	}
}

bool	Channel::isOperator(const Client &user) const
{
	return _operators.find(const_cast<Client *>(&user)) != _operators.end();
}

string	Channel::getMembersString(void) const
{
	string	members_str;

	members_str.reserve(_members.size() * 10);
	for (set<Client *>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
		members_str += "@" + (*it)->getNickname() + ", ";
	for (map<string, Client *>::const_iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (!isOperator(*it->second))
			members_str += it->first + ", ";
	}
	members_str.erase(members_str.length() - 2);
	return members_str;
}

void	Channel::checkName(const string &name) const
{
	if (!is_valid_channel_name(name))
		throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name, name + " is not a valid channel name");
}

void	Channel::checkKey(const string &key) const
{
	if (key != _key)
		throw ProtocolErrorException(ERR_BADCHANNELKEY, _name, key + " is not a valid channel key");
}
