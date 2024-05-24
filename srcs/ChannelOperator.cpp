/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 17:50:10 by craimond         ###   ########.fr       */
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
		map<string, Client *>			members = channel.getMembers();
		const string					&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) == members.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERNOTINCHANNEL, nickname.c_str(), channel.getName().c_str()));
		channel.removeMember(user);
		user.removeChannel(channel);

		const struct s_commandContent message_to_target = EventHandler::buildCommandContent("", reason, KICK, channel.getName().c_str(), _nickname.c_str());
		EventHandler::sendBufferedContent(user, &message_to_target);
	}

	void ChannelOperator::invite(Client &user, Channel &channel) const
	{
		map<string, Client *>			members = channel.getMembers();
		const string					&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) != members.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERONCHANNEL, nickname.c_str(), channel.getName().c_str()));
		channel.addPendingInvitation(&user);

		const struct s_replyContent		reply_to_issuer = EventHandler::buildReplyContent("", RPL_INVITING, nickname.c_str(), channel.getName().c_str());
		EventHandler::sendBufferedContent(*this, &reply_to_issuer);

		const struct s_commandContent	message_to_target = EventHandler::buildCommandContent(_nickname, "", INVITE, nickname.c_str(), channel.getName().c_str());
		EventHandler::sendBufferedContent(user, &message_to_target);
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
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_CHANOPRIVSNEEDED, channel.getName().c_str()));
	}
}