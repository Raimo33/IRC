/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 16:41:20 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ChannelOperator.hpp"

ChannelOperator::ChannelOperator(const User &user) : User(user) {}

ChannelOperator::ChannelOperator(const ChannelOperator &copy) : User(copy) {}

ChannelOperator::~ChannelOperator() {}

void ChannelOperator::channelKick(const User &user, Channel &channel) const
{
	std::map<std::string, User *>				members = channel.getMembers();
	std::map<std::string, ChannelOperator *>	operators = channel.getOperators();

	if (members.find(user.getNickname()) == members.end())
		throw UserNotInChannelException();
	if (operators.find(getNickname()) == operators.end())
		throw OperatorNotInChannelException();
	channel.removeMember(user);
}

void ChannelOperator::channelInvite(User &user, Channel &channel) const
{
	std::map<std::string, ChannelOperator *>	operators = channel.getOperators();

	if (operators.find(getNickname()) == operators.end())
		throw Channel::UserNotOperatorException();
	channel.addPendingInvitation(&user);
}

std::string ChannelOperator::channelTopicGet(const Channel &channel) const
{
	std::map<std::string, ChannelOperator *>	operators = channel.getOperators();

	if (operators.find(getNickname()) == operators.end())
		throw OperatorNotInChannelException();
	return channel.getTopic();
}

void ChannelOperator::channelTopicSet(Channel &channel, const std::string &new_topic) const
{
	std::map<std::string, ChannelOperator *>	operators = channel.getOperators();

	if (operators.find(getNickname()) == operators.end())
		throw OperatorNotInChannelException();
	channel.setTopic(new_topic);
}

void ChannelOperator::channelModeChange(Channel &channel, const t_channel_modes &mode, const bool status) const
{
	//TODO implementare
	(void)channel;
	(void)mode;
	(void)status;
}

const char *ChannelOperator::UserNotInChannelException::what() const throw()
{
	return "User is not in the channel";
}

const char *ChannelOperator::OperatorNotInChannelException::what() const throw()
{
	return "Operator is not in the channel";
}