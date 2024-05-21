/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ChannelOperator.hpp"
#include "irc/Exceptions.hpp"

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
		throw UserAlreadyMemberException();
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
		throw OperatorNotInChannelException();
	if (operators.find(_username) == operators.end())
		throw UserNotOperatorException();
}
