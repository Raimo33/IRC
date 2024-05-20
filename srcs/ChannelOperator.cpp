/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 11:39:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ChannelOperator.hpp"

ChannelOperator::ChannelOperator(const User &user) : User(user) {}

ChannelOperator::ChannelOperator(const ChannelOperator &copy) : User(copy) {}

ChannelOperator::~ChannelOperator() {}

void ChannelOperator::channelKick(const User &user, Channel &channel) const
{
	map<string, User *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	checkPrivilege(channel);
	if (members.find(user.getUsername()) == members.end())
		throw UserNotInChannelException();
	channel.removeMember(user);
}

void ChannelOperator::channelInvite(User &user, Channel &channel) const
{
	map<string, User *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	checkPrivilege(channel);
	if (members.find(user.getUsername()) != members.end())
		throw Channel::UserAlreadyMemberException();
	channel.addPendingInvitation(&user);
}

void ChannelOperator::channelTopicSet(Channel &channel, const string &new_topic) const
{
	map<string, User *>				members = channel.getMembers();
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
	map<string, User *>				members = channel.getMembers();
	map<string, ChannelOperator *>	operators = channel.getOperators();

	if (members.find(_username) == members.end())
		throw OperatorNotInChannelException();
	if (operators.find(_username) == operators.end())
		throw Channel::UserNotOperatorException();
}

const char *ChannelOperator::UserNotInChannelException::what() const throw()
{
	return "User is not in the channel";
}

const char *ChannelOperator::OperatorNotInChannelException::what() const throw()
{
	return "Operator is not in the channel";
}