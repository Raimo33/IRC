/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 12:36:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ChannelOperator.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"

using std::map;
using std::string;

namespace irc
{
	ChannelOperator::ChannelOperator(const Client &user) : Client(user) {}

	ChannelOperator::ChannelOperator(const ChannelOperator &copy) : Client(copy) {}

	ChannelOperator::~ChannelOperator() {}

	void ChannelOperator::kick(const Client &user, Channel &channel) const
	{
		map<string, Client *>			members = channel.getMembers();
		map<string, ChannelOperator *>	operators = channel.getOperators();
		const string					&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) == members.end())
			throw ProtocolErrorException("", ERR_USERNOTINCHANNEL, nickname.c_str(), channel.getName().c_str());
		channel.removeMember(user);
	}

	void ChannelOperator::invite(Client &user, Channel &channel) const
	{
		map<string, Client *>			members = channel.getMembers();
		map<string, ChannelOperator *>	operators = channel.getOperators();
		const string					&nickname = user.getNickname();

		checkPrivilege(channel);
		if (members.find(nickname) != members.end())
			throw ProtocolErrorException("", ERR_USERONCHANNEL, nickname.c_str(), channel.getName().c_str());
		channel.addPendingInvitation(&user);
	}

	void ChannelOperator::topicSet(Channel &channel, const string &new_topic) const
	{
		map<string, Client *>			members = channel.getMembers();
		map<string, ChannelOperator *>	operators = channel.getOperators();

		checkPrivilege(channel);
		channel.setTopic(new_topic);
	}

	//MODE #channel +n params
	void ChannelOperator::modeChange(Channel &channel, const t_channel_modes &mode, const bool status) const
	{
		//TODO implementare
		(void)channel;
		(void)mode;
		(void)status;
	}

	void ChannelOperator::checkPrivilege(const Channel &channel) const
	{
		map<string, Client *>			members = channel.getMembers();
		map<string, ChannelOperator *>	operators = channel.getOperators();

		if (members.find(_username) == members.end())
			throw ProtocolErrorException("", ERR_NOTONCHANNEL, channel.getName().c_str());
		if (operators.find(_username) == operators.end())
			throw ProtocolErrorException("", ERR_CHANOPRIVSNEEDED, channel.getName().c_str());
	}
}