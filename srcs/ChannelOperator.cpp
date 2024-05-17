/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 15:43:31 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ChannelOperator.hpp"

ChannelOperator::ChannelOperator() : User() {}

ChannelOperator::ChannelOperator(const ChannelOperator &copy) : User(copy) {}

ChannelOperator::ChannelOperator(const User &user) : User(user) {}

ChannelOperator::~ChannelOperator() {}

ChannelOperator &ChannelOperator::operator=(const ChannelOperator &rhs)
{
	if (this != &rhs)
		User::operator=(rhs);
	return *this;
}

void	ChannelOperator::channelKick(const User &user, Channel &channel) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw UserNotInChannelException();
	channel.removeUser(user);
}

void	ChannelOperator::channelInvite(User &user, Channel &channel) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw OperatorNotInChannelException();
	channel.addUser(user);
}

string	ChannelOperator::channelTopicGet(const Channel &channel) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw OperatorNotInChannelException();
	return channel.getTopic();
}

void	ChannelOperator::channelTopicSet(Channel &channel, const string &new_topic) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw OperatorNotInChannelException();
	channel.setTopic(new_topic);
}

void	ChannelOperator::channelModeChange(Channel &channel, const t_channel_modes &mode, const bool status) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw OperatorNotInChannelException();
	channel.setMode(mode, status);
}

const char *ChannelOperator::UserNotInChannelException::what() const throw()
{
	return "User is not in the channel";
}

const char *ChannelOperator::OperatorNotInChannelException::what() const throw()
{
	return "Operator is not in the channel";
}