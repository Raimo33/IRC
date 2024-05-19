/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 16:47:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/User.hpp"
#include "headers/Channel.hpp"
#include "headers/Message.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Server.hpp"
#include "headers/utils.hpp"

User::User(void) :
	_pwd_hash(0),
	_is_authenticated(false) {}

User::User(const User &copy) :
	_channels(copy._channels),
	_nickname(copy._nickname),
	_username(copy._username),
	_pwd_hash(copy._pwd_hash),
	_is_authenticated(copy._is_authenticated) {}

User::~User(void) {}

const map<string, const Channel *>	&User::getChannels(void) const
{
	return _channels;
}

void	User::setChannels(const map<string, const Channel *> &channels)
{
	_channels = channels;
}

const Channel	*User::getChannel(const string &channel_name) const
{
	map<string, const Channel *>::const_iterator it = _channels.find(channel_name);

	if (it == _channels.end())
		throw UserNotInChannelException();
	return it->second;
}

void	User::addChannel(const Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw TooManyChannelsException();
	_channels[channel.getName()] = &channel;
}

void	User::removeChannel(const Channel &channel)
{
	map<string, const Channel *>::iterator it = _channels.find(channel.getName());

	if (it == _channels.end())
		throw UserNotInChannelException();
	_channels.erase(it);
}

const string	&User::getNickname(void) const
{
	return _nickname;
}

void	User::setNickname(const string &nickname)
{
	_nickname = nickname;
}

const string	&User::getUsername(void) const
{
	return _username;
}

void	User::setUsername(const string &username)
{
	_username = username;
}

const string	&User::getPwdHash(void) const
{
	return _pwd_hash;
}

void	User::setPwdHash(const string &pwd_hash)
{
	_pwd_hash = pwd_hash;
}

bool	User::getIsAuthenticated(void) const
{
	return _is_authenticated;
}

void	User::setAuthenticated(bool is_authenticated)
{
	if (_is_authenticated == is_authenticated)
	{
		if (is_authenticated)
			throw AlreadyAuthenticatedException();
		else
			throw AlreadyAuthenticatedException();
	}
	_is_authenticated = is_authenticated;
}

void	User::joinChannel(Channel &channel)
{
	if (!_is_authenticated)
		throw NotAuthenticatedException();
	try
	{
		channel.addMember(*this);
		addChannel(channel);
	}
	catch (const TooManyChannelsException &e) //catcho il fallimento di addChannel
	{
		//annullo il successo di addMember
		channel.removeMember(*this);
	}
}

void	User::joinChannel(Channel &channel, const string &key)
{
	if (!_is_authenticated)
		throw NotAuthenticatedException();
	if (channel.getKey() != key)
		throw Channel::InvalidKeyException();
	joinChannel(channel);
}

void	User::leaveChannel(Channel &channel)
{
	if (!_is_authenticated)
		throw NotAuthenticatedException();
	channel.removeMember(*this);
	removeChannel(channel);
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

const char *User::UserNotInChannelException::what() const throw()
{
	return "User is not in the channel";
}
