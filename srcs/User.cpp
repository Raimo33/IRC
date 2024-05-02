/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 15:38:46 by craimond         ###   ########.fr       */
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

//questa sara' chiamata solo se l'username non e' nel database (l'utente non e' gia' registrato)
/*Because of IRC's scandanavian origin, the characters {}| are
   considered to be the lower case equivalents of the characters []\,
   respectively. This is a critical issue when determining the
   equivalence of two nicknames.*/
void	User::authenticate(const string &username, const string &password)
{
	if (_is_authenticated == true)
		throw AlreadyAuthenticatedException();

	hash<string>	hasher;

	_username = username;
	_pwd_hash = hasher(password);
	_is_authenticated = true;
}

void	User::setNickname(const string &nickname)
{
	_nickname = nickname;
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
