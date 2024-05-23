/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 12:26:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ChannelOperator.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"

using namespace std;
using namespace irc;

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
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, nickname, channel.getName());
	channel.removeMember(user);
}

void ChannelOperator::invite(Client &user, Channel &channel) const
{
	map<string, Client *>			members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();
	const string					&nickname = user.getNickname();

	checkPrivilege(channel);
	if (members.find(nickname) != members.end())
		throw ProtocolErrorException(ERR_USERONCHANNEL, nickname, channel.getName());
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
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel.getName());
	if (operators.find(_username) == operators.end())
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, channel.getName());
}
