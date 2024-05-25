/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 17:59:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ChannelOperator.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"
#include "irc/EventHandler.hpp"

using std::map;
using std::string;

namespace irc
{
	ChannelOperator::ChannelOperator(const Client &user) : Client(user) {}

	ChannelOperator::ChannelOperator(const ChannelOperator &copy) : Client(copy) {}

	ChannelOperator::~ChannelOperator() {}

	void ChannelOperator::kick(Client &user, Channel &channel, const string &reason) const
	{
		map<string, Client *>	members = channel.getMembers();
		const string			&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) == members.end())
		{
			const string params[] = { nickname, channel.getName() };
			throw ProtocolErrorException(ERR_USERNOTINCHANNEL, params);
		}
		channel.removeMember(user);
		user.removeChannel(channel);

		const string params[] = { channel.getName(), _nickname };
		const struct s_commandContent message_to_channel = EventHandler::buildCommandContent("", KICK, params, reason);
		channel.receiveMessage(message_to_channel);
	}

	void ChannelOperator::invite(Client &user, Channel &channel) const
	{
		map<string, Client *>	members = channel.getMembers();
		const string			&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) != members.end())
		{
			const string params[] = { nickname, channel.getName() };
			throw ProtocolErrorException(ERR_USERONCHANNEL, params);
		}
		channel.addPendingInvitation(&user);
		{
			const string params[] = { nickname, channel.getName() };
			const struct s_replyContent reply_to_issuer = EventHandler::buildReplyContent(RPL_INVITING, params);
			EventHandler::sendBufferedContent(*this, &reply_to_issuer);
		}
		{
			const string params[] = { nickname, channel.getName() };
			const struct s_commandContent message_to_target = EventHandler::buildCommandContent(_nickname, INVITE, params);
			EventHandler::sendBufferedContent(user, &message_to_target);
		}
	}

	void ChannelOperator::topicSet(Channel &channel, const string &new_topic) const
	{
		checkPrivilege(channel);
		channel.setTopic(new_topic, *this);
	}

	//MODE #channel +n params
	void ChannelOperator::modeChange(Channel &channel, const t_channel_modes &mode, const bool status) const
	{
		//TODO implementare
		(void)channel;
		(void)mode;
		(void)status;
	}

	void ChannelOperator::promoteOperator(Channel &channel, const Client &user) const
	{
		checkPrivilege(channel);
		ChannelOperator op(user);
		channel.addOperator(op);
	}

	void ChannelOperator::demoteOperator(Channel &channel, ChannelOperator &op) const
	{
		checkPrivilege(channel);
		channel.removeOperator(op);
	}

	void ChannelOperator::checkPrivilege(const Channel &channel) const
	{
		if (channel.isOperator(this) == false)
			throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, channel.getName());
	}
}