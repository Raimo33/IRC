/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 11:23:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/User.hpp"
#include "headers/Channel.hpp"
#include "headers/Message.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Server.hpp"
#include "headers/utils.hpp"

User::User(void) :
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

User::~User(void) {}

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
	if (channel.handleJoinRequest(*this))
		_channels[channel.getName()] = &channel;
}

void	User::joinChannel(Channel &channel, const string &key)
{
	if (_channels.size() > MAX_CHANNELS_PER_USER)
		throw TooManyChannelsException();
	if (channel.getMode(MODE_K) == false || channel.getKey() == key)
	{
		if (channel.handleJoinRequest(*this))
			_channels[channel.getName()] = &channel;
	}
	else
		throw InvalidCredentialsException();
}

string	User::getNickname(void) const
{
	return _nickname;
}

string	User::getUsername(void) const
{
	return _username;
}

bool	User::isAuthenticated(void) const
{
	return _is_authenticated;
}

const char *User::TooManyChannelsException::what() const throw()
{
	return "User is already in too many channels";
}

const char *User::AlreadyAuthenticatedException::what() const throw()
{
	return "User is already authenticated";
}

const char *User::InvalidCredentialsException::what() const throw()
{
	return "Invalid credentials";
}

const char *User::NotAuthenticatedException::what() const throw()
{
	return "User is not authenticated";
}

