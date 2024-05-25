/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 12:58:50 by craimond         ###   ########.fr       */
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

using std::string;
using std::map;

namespace irc
{
	Channel::Channel(const string &name, ChannelOperator &op) :
		_name(name),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
	{
		checkName(name);
		//_operators[op.getNickname()] = &op;
		_members[op.getNickname()] = &op;
		for (int i = 0; i < N_MODES; i++)
			_modes[i] = false;
	}

	Channel::Channel(const string &name, const string &key, ChannelOperator &op) :
		_name(name),
		_key(key),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT)
	{
		checkName(name);
		//_operators[op.getNickname()] = &op;
		_members[op.getNickname()] = &op;
		for (int i = 0; i < N_MODES; i++)
			_modes[i] = false;
		_modes[MODE_K] = true;
	}

	Channel::Channel(const Channel &copy) :
		_name(copy._name),
		_topic(copy._topic),
		//_operators(copy._operators),
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
		checkName(new_name);
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
	}

	const string &Channel::getTopic(void) const
	{
		if (_topic.empty())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", RPL_NOTOPIC, _name.c_str()));
		return _topic;
	}

	void Channel::setTopic(const string &new_topic, const Client &setter)
	{
		const string &nickname = setter.getNickname();

		if (_modes[MODE_T] && isOperator(&setter))
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_CHANOPRIVSNEEDED, nickname.c_str(), _name.c_str()));
		if (new_topic.length() > MAX_CHANNEL_TOPIC_LEN)
			throw ProtocolErrorException(EventHandler::buildReplyContent(new_topic + " is too long", RPL_NOTOPIC, _name.c_str()));
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

	void Channel::addOperator(ChannelOperator &op)
	{
		string	nickname = op.getNickname();

		if (isOperator(nickname) == true)
			throw ProtocolErrorException(EventHandler::buildReplyContent(nickname + " is already an operator of " + _name, ERR_USERONCHANNEL, nickname.c_str(), _name.c_str()));
		_members[nickname] = &op; //sovrascrivo il membro con l'operator (non posso avere due membri con lo stesso nickname)

		const s_replyContent youreoper = EventHandler::buildReplyContent("", RPL_YOUREOPER, nickname.c_str());
		EventHandler::sendBufferedContent(op, &youreoper);
	}

	void Channel::removeOperator(ChannelOperator &op)
	{
		string							nickname = op.getNickname();

		if (isOperator(nickname) == false)
			throw ProtocolErrorException(EventHandler::buildReplyContent(nickname + " is not an operator of " + _name, ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
		_members.erase(nickname);
		addMember(op); //aggiungo l'operator come membro normale

		const struct s_replyContent notoperanymore = EventHandler::buildReplyContent("", RPL_NOTOPERANYMORE);
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
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
		return *(_members.at(nickname));
	}

	void Channel::addMember(Client &user)
	{
		const string &nickname = user.getNickname();

		if (_modes[MODE_I] && _pending_invitations.find(nickname) == _pending_invitations.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_INVITEONLYCHAN, _name.c_str()));
		if (_members.find(nickname) != _members.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERONCHANNEL, nickname.c_str(), _name.c_str()));
		if (_members.size() >= _member_limit)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_CHANNELISFULL, _name.c_str()));
		_members[nickname] = &user;
	}

	void Channel::removeMember(const Client &user)
	{
		const string &nickname = user.getNickname();
		
		if (_members.find(nickname) == _members.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
		_members.erase(nickname);
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
			throw ProtocolErrorException(EventHandler::buildReplyContent(nickname + " was not invited to " + _name, ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
		return *(_pending_invitations.at(nickname));
	}

	void Channel::addPendingInvitation(Client *user)
	{
		const string &nickname = user->getNickname();

		if (_pending_invitations.find(nickname) != _pending_invitations.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent(nickname + " is already invited to " + _name, ERR_USERONCHANNEL, nickname.c_str(), _name.c_str()));
		_pending_invitations[nickname] = user;
	}

	void Channel::removePendingInvitation(const Client &user)
	{
		const string &nickname = user.getNickname();

		if (_pending_invitations.find(nickname) == _pending_invitations.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent(nickname + " was not invited to " + _name, ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
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

	void	Channel::receiveMessage(const Message &msg) const
	{

		for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
		{
			const struct s_commandContent msg_content = EventHandler::buildCommandContent(msg.getSender().getNickname(), msg.getText(), PRIVMSG, _name.c_str());
			EventHandler::sendBufferedContent(*(receiver->second), &msg_content);
		}
	}

	bool	Channel::isOperator(const string &nickname) const
	{
		map<string, Client *>::const_iterator it = _members.find(nickname);

		if (it == _members.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERNOTINCHANNEL, nickname.c_str(), _name.c_str()));
		return (dynamic_cast<const ChannelOperator *>(it->second) != NULL);
	}

	bool	Channel::isOperator(const Client *user) const
	{
		return isOperator(user->getNickname());
	}

	string	Channel::getMembersString(void) const //TODO ottimizzare, mantenere una stringa aggiornata di operatori e membri
	{
		string	members_str;

		members_str.reserve(_members.size() * 10);
		for (map<string, Client *>::const_iterator it = _members.begin(); it != _members.end(); it++)
		{
			if (isOperator(it->first) == true)
				members_str += "@" + it->first + ", ";
		}
		for (map<string, Client *>::const_iterator it = _members.begin(); it != _members.end(); it++)
		{
			if (isOperator(it->first) == false)
				members_str += it->first + ", ";
		}
		members_str.erase(members_str.length() - 2);
		return members_str;
	}

	void	Channel::checkName(const string &name) const
	{
		if (is_valid_channel_name(name) == false)
			throw ProtocolErrorException(EventHandler::buildReplyContent(name + " is not a valid channel name", ERR_NOSUCHCHANNEL, name.c_str()));
	}

	void	Channel::checkKey(const string &key) const
	{
		if (key != _key)
			throw ProtocolErrorException(EventHandler::buildReplyContent(key + " is not a valid channel key", ERR_BADCHANNELKEY, _name.c_str()));
	}
}
