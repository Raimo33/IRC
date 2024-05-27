/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:00:46 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 19:13:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Channel.hpp"
#include "irc/Client.hpp"
#include "irc/ChannelOperator.hpp"
#include "irc/utils.hpp"
#include "irc/EventHandler.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/Constants.hpp"
#include "irc/Content.hpp"
#include "irc/Logger.hpp"

#include <sstream>

using std::string;
using std::map;
using std::vector;
using std::stringstream;

namespace irc
{
	Channel::Channel(Logger &logger, const string &name, Client &op) :
		_name(name),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT),
		_modes(256, false),
		_logger(logger)
	{
		checkName(name);
		op.joinChannel(*this);
		_logger.logEvent("Channel created: " + name);
	}

	Channel::Channel(Logger &logger, const string &name, const string &key, Client &op) :
		_name(name),
		_key(key),
		_member_limit(DEFAULT_CHANNEL_MEMBER_LIMIT),
		_modes(256, false),
		_logger(logger)
	{
		checkName(name);
		op.joinChannel(*this);
		_modes['k'] = true;
		_logger.logEvent("Channel created: " + name);
	}

	Channel::Channel(const Channel &copy) :
		_name(copy._name),
		_key(copy._key),
		_topic(copy._topic),
		_member_limit(copy._member_limit),
		_members(copy._members),
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

	void Channel::setTopic(const string &new_topic, const Client &setter)
	{
		const string &nickname = setter.getNickname();

		if (_modes['t'] && isOperator(&setter))
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, params);	
		}
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

	void Channel::addOperator(ChannelOperator &op)
	{
		string	nickname = op.getNickname();

		if (isOperator(nickname) == true)
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERONCHANNEL, params, nickname + " is already an operator of " + _name);
		}
		_members[nickname] = &op; //sovrascrivo il membro con l'operator (non posso avere due membri con lo stesso nickname)

		_logger.logEvent("Channel " + _name + ", operator added: " + nickname);
		const struct s_replyContent youreoper = EventHandler::buildReplyContent(RPL_YOUREOPER, nickname);
		op.receiveMessage(youreoper);
	}

	void Channel::removeOperator(ChannelOperator &op)
	{
		string	nickname = op.getNickname();

		if (!isOperator(nickname))
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params, nickname + " is not an operator of " + _name);	
		}
		_members.erase(nickname);
		_members[nickname] = &op;

		_logger.logEvent("Channel " + _name + ", operator removed: " + nickname);
		const struct s_replyContent notoperanymore = EventHandler::buildReplyContent(RPL_NOTOPERANYMORE);
		op.receiveMessage(notoperanymore);
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
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);
		}
		return *(_members.at(nickname));
	}

	void Channel::addMember(Client &user)
	{
		const string &nickname = user.getNickname();

		if (_modes['i'] && _pending_invitations.find(nickname) == _pending_invitations.end())
			throw ProtocolErrorException(ERR_INVITEONLYCHAN, _name);
		if (_members.find(nickname) != _members.end())
			throw ProtocolErrorException(ERR_USERONCHANNEL, _name);	
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
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);	
		}
		_members.erase(nickname);
		_logger.logEvent("Channel " + _name + ", member removed: " + nickname);
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
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params, nickname + " was not invited to " + _name);
		}
		return *(_pending_invitations.at(nickname));
	}

	void Channel::addPendingInvitation(Client *user)
	{
		const string &nickname = user->getNickname();

		if (_pending_invitations.find(nickname) != _pending_invitations.end())
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERONCHANNEL, params, nickname + " is already invited to " + _name);
		}
		_pending_invitations[nickname] = user;
		_logger.logEvent("Channel " + _name + ", invitation sent to: " + nickname);
	}

	void Channel::removePendingInvitation(const Client &user)
	{
		const string &nickname = user.getNickname();

		if (_pending_invitations.find(nickname) == _pending_invitations.end())
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params, nickname + " was not invited to " + _name);
		}
		_pending_invitations.erase(nickname);
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
				if (params.size() < j + 1)
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
			ChannelOperator	op(getMember(param));

			status ? addOperator(op) : removeOperator(op);
		}
		else
			_logger.logEvent("Channel " + _name + ", mode " + mode + " is now " + (status ? "on" : "off"));

		string mode_str = (status ? "+" : "-") + mode;
		const string params[] = { _name, mode_str, param };		
		const struct s_commandContent mode_change = EventHandler::buildCommandContent(SERVER_NAME, MODE, params);
		receiveMessage(mode_change);
	}

	void	Channel::receiveMessage(const struct s_commandContent &msg) const
	{
		for (map<string, Client *>::const_iterator receiver = _members.begin(); receiver != _members.end(); receiver++)
		{
			if (receiver->first != msg.prefix)
				receiver->second->receiveMessage(msg);
		}
	}

	bool	Channel::isOperator(const string &nickname) const
	{
		map<string, Client *>::const_iterator it = _members.find(nickname);

		if (it == _members.end())
		{
			const string params[] = { nickname, _name };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);
		}
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
			if (!isOperator(it->first))
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
}
