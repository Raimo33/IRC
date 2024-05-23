/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 18:50:17 by craimond         ###   ########.fr       */
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
#include "irc/Constants.hpp"
#include "irc/Content.hpp"

using namespace std;

namespace irc
{
	Channel::Channel(const string &name, ChannelOperator &op) :
		_name(name),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
	{
		if (!is_valid_channel_name(name))
			throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name.c_str()); //TODO custom message (name + is not a valid channel name)
		_operators[op.getNickname()] = &op;
		_members[op.getNickname()] = &op;
		for (int i = 0; i < N_MODES; i++)
			_modes[i] = false;
	}

	Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
		_name(name),
		_key(key),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
	{
		if (!is_valid_channel_name(name))
			throw ProtocolErrorException(ERR_NOSUCHCHANNEL, name.c_str()); //TODO custom message (name + is not a valid channel name)
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
			throw ProtocolErrorException(ERR_NOSUCHCHANNEL, new_name.c_str()); //TODO custom message (new_name + is not a valid channel name)
		_name = new_name;
	}

	const string &Channel::getKey(void) const
	{
		return _key;
	}

	void Channel::setKey(const string &new_key)
	{
		if (new_key.empty() || new_key.length() > MAX_CHANNEL_KEY_LEN)
			throw ProtocolErrorException(ERR_KEYSET, _name.c_str()); //TODO messaggio custom (key + is not a valid key)
		_key = new_key;
	}

	const string &Channel::getTopic(void) const
	{
		if (_topic.empty())
			throw ProtocolErrorException(RPL_NOTOPIC, _name.c_str());
		return _topic;
	}

	void Channel::setTopic(const string &new_topic)
	{
		if (new_topic.length() > MAX_CHANNEL_TOPIC_LEN)
			throw ProtocolErrorException(RPL_NOTOPIC, _name.c_str()); //TODO messaggio custom (topic + is too long)
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
			throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, nickname.c_str(), _name.c_str()); //TODO custom message (nickname + is not an operator of + channel name)
		return *(_operators.at(nickname));
	}

	void Channel::addOperator(ChannelOperator *op)
	{
		string nickname = op->getNickname();

		if (_operators.find(nickname) != _operators.end())
			throw ProtocolErrorException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str()); //TODO messaggio custom (nickname + is already an operator of + channel name)
		if (_members.find(nickname) == _members.end())
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str());
		_operators[nickname] = op;

		struct s_replyContent youreoper;

		youreoper.prefix = string(SERVER_NAME);
		youreoper.code = RPL_YOUREOPER;
		youreoper.params.push_back(nickname);

		EventHandler::sendBufferedContent(*op, &youreoper);
	}

	void Channel::removeOperator(const ChannelOperator &op)
	{
		string nickname = op.getNickname();

		if (_operators.find(nickname) == _operators.end())
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()); //TODO messaggio custom (nickname + is not an operator of + channel name)
		delete _operators[nickname];
		_operators.erase(nickname);

		struct s_replyContent notoperanymore;

		notoperanymore.prefix = string(SERVER_NAME);
		notoperanymore.code = RPL_NOTOPERANYMORE;
		notoperanymore.params.push_back(nickname);

		EventHandler::sendBufferedContent(op, &notoperanymore);
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
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str());
		return *(_members.at(nickname));
	}

	void Channel::addMember(Client &user)
	{
		const string &nickname = user.getNickname();

		if (_modes[MODE_I] && _pending_invitations.find(nickname) == _pending_invitations.end())
			throw ProtocolErrorException(ERR_INVITEONLYCHAN, _name.c_str());
		if (_members.find(nickname) != _members.end())
			throw ProtocolErrorException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str());
		if (_members.size() >= _member_limit)
			throw ProtocolErrorException(ERR_CHANNELISFULL, _name.c_str());
		_members[nickname] = &user;
		if (!_members_string.empty())
			_members_string += " ";
		_members_string += nickname;
	}

	void Channel::removeMember(const Client &user)
	{
		const string &nickname = user.getNickname();
		
		if (_members.find(nickname) == _members.end())
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str());
		_members.erase(nickname);
		_members_string.erase(_members_string.find(nickname), nickname.length() + 1);
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
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()); //TODO custom message (nickname + was not invited to + channel name)
		return *(_pending_invitations.at(nickname));
	}

	void Channel::addPendingInvitation(Client *user)
	{
		const string &nickname = user->getNickname();
		
		if (_pending_invitations.find(nickname) != _pending_invitations.end())
			throw ProtocolErrorException(ERR_USERONCHANNEL, nickname.c_str(), _name.c_str()); //TODO custom message (nickname + is already invited to + channel name)
		_pending_invitations[nickname] = user;

		struct s_replyContent inviting;

		inviting.prefix = string(SERVER_NAME);
		inviting.code = RPL_INVITING;
		inviting.params.push_back(_name);
		inviting.params.push_back(nickname);
		inviting.text = "You have been invited to " + _name;

		EventHandler::sendBufferedContent(*user, &inviting);
	}

	void Channel::removePendingInvitation(const Client &user)
	{
		const string &nickname = user.getNickname();

		if (_pending_invitations.find(nickname) == _pending_invitations.end())
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()); //TODO custom message (nickname + was not invited to + channel name)
		_pending_invitations.erase(nickname);
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

	const string &Channel::getMembersString(void) const
	{
		return _members_string;
	}

	void Channel::setMembersString(const string new_members_string)
	{
		_members_string = new_members_string;
	}

	void	Channel::receiveMessage(const Message &msg) const
	{

		for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
		{
			struct s_commandContent msg_content;

			msg_content.prefix = ":" + msg.getSender().getNickname();
			msg_content.cmd = PRIVMSG;
			msg_content.params.push_back(_name);
			msg_content.text = msg.getText();
			EventHandler::sendBufferedContent(*(receiver->second), &msg_content);
		}
	}
}
