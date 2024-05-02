/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:00:22 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 12:08:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ChannelOperator.hpp"

ChannelOperator::ChannelOperator() : User() {}

ChannelOperator::ChannelOperator(const ChannelOperator &copy) : User(copy) {}

ChannelOperator::~ChannelOperator() {}

ChannelOperator &ChannelOperator::operator=(const ChannelOperator &rhs)
{
	if (this != &rhs)
		User::operator=(rhs);
	return *this;
}

void	ChannelOperator::channelKick(const User &user, const Channel &channel) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw UserNotInChannelException();
	channel.removeUser(user);
}