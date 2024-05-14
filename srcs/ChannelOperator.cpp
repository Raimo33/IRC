/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 16:46:12 by craimond         ###   ########.fr       */
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

const char *ChannelOperator::UserNotInChannelException::what() const throw()
{
	return "User is not in the channel";
}