/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 15:48:30 by craimond         ###   ########.fr       */
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

void ChannelOperator::channelKick(const Client &user, Channel &channel) const
{
	map<string, Client *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	checkPrivilege(channel);
	if (members.find(user.getUsername()) == members.end())
		throw UserNotMemberException();
	channel.removeMember(user);
}

void ChannelOperator::channelInvite(Client &user, Channel &channel) const
{
	map<string, Client *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	checkPrivilege(channel);
	if (members.find(user.getUsername()) != members.end())
	{
		vector<string> params(2);

		params.push_back(user.getUsername());
		params.push_back(channel.getName());
		throw ProtocolErrorException(ERR_USERONCHANNEL, params);
	}
	channel.addPendingInvitation(&user);
}

void ChannelOperator::channelTopicSet(Channel &channel, const string &new_topic) const
{
	map<string, Client *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	checkPrivilege(channel);
	channel.setTopic(new_topic);
}

//MODE #channel +n params
void ChannelOperator::channelModeChange(Channel &channel, const t_channel_modes &mode, const bool status) const
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
		throw ProtocolErrorException(ERR_NOTONCHANNEL, vector<string>(1, channel.getName()));
	if (operators.find(_username) == operators.end())
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, vector<string>(1, channel.getName()));
}
