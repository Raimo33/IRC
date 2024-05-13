/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 18:28:54 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/User.hpp"

User::User() :
	_channels(),
	_nickname(""),
	_username(""),
	_pwd_hash(0),
	_is_authenticated(false) {}

User::User(const User &copy) :
	_channels(copy._channels),
	_nickname(copy._nickname),
	_username(copy._username),
	_pwd_hash(copy._pwd_hash),
	_is_authenticated(copy._is_authenticated) {}

User::~User() {}

User &User::operator=(const User &rhs)
{
	if (this != &rhs)
	{
		_channels = rhs._channels;
		_nickname = rhs._nickname;
		_username = rhs._username;
		_pwd_hash = rhs._pwd_hash;
		_is_authenticated = rhs._is_authenticated;
	}
	return *this;
}

void	User::setNickname(const string &nickname)
{
	_nickname = nickname;
}

void	User::setUsername(const string &username)
{
	_username = username;
}

// questa funzione viene chiamata solo se il canale esiste gia'
void	User::joinChannel(Channel &channel)
{
	if (_channels.size() > MAX_CHANNELS_PER_USER)
		throw TooManyChannelsException();
	if (channel.getMode(MODE_I) == false) //se il canale non e' invite-only
		_channels[channel.getName()] = &channel;
	else
		channel.addRequest(*this);
}

void	User::joinChannel(Channel &channel, const string &key)
{
	if (_channels.size() > MAX_CHANNELS_PER_USER)
		throw TooManyChannelsException();
	if (channel.getMode(MODE_K) == false || channel.getKey() == key)
		_channels[channel.getName()] = &channel;
	else
		throw Channel::InvalidCredentialsException();
}

