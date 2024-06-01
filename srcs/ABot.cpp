/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 16:25:43 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ABot.hpp"
#include "irc/Channel.hpp"
#include "irc/Message.hpp"

ABot::ABot(const std::string &nickname, const std::string &username, const std::string &realname):
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_socket(-1),
	_connected(false) {}

ABot::ABot(const ABot &copy):
	_nickname(copy._nickname),
	_username(copy._username),
	_realname(copy._realname),
	_socket(copy._socket),
	_connected(copy._connected),
	_logger(copy._logger) {}

ABot::~ABot(void) {}

const std::string	&ABot::getNickname(void) const { return _nickname; }
const std::string	&ABot::getUsername(void) const { return _username; }
const std::string	&ABot::getRealname(void) const { return _realname; }
bool				ABot::isConnected(void) const { return _connected; }
int					ABot::getSocket(void) const { return _socket; }
void				ABot::setLogger(const Logger &logger) { _logger = logger; }
const Logger		&ABot::getLogger(void) const { return _logger; }
void				ABot::setConnected(bool connected) { _connected = connected; }
void				ABot::setSocket(int socket) { _socket = socket; }

void				ABot::joinChannel(Channel &channel, const std::string &key)
{
	if (_channels.find(channel.getName()) != _channels.end())
		throw IllegalActionException("Bot is already in channel " + channel.getName());

	Message msg(_nickname, JOIN, channel.getName(), key);
	channel.receiveMessage(msg);
	_channels[channel.getName()] = &channel;
	_logger.logEvent("Bot joined channel " + channel.getName());
}

void				ABot::sendMessage(const Channel &channel, const std::string &text)
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw IllegalActionException("Bot is not in channel " + channel.getName());

	Message msg(_nickname, PRIVMSG, channel.getName(), text);
	channel.receiveMessage(msg);
	_logger.logEvent("Bot sent message to channel " + channel.getName());
}

