/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 16:45:40 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "EventHandler.hpp"
#include "system_calls.hpp"

using std::string;

ABot::ABot(const string &nickname, const string &username, const string &realname) : _nickname(nickname),
																					 _username(username),
																					 _realname(realname),
																					 _socket(-1),
																					 _connected(false) {}

ABot::ABot(const ABot &copy) : _nickname(copy._nickname),
							   _username(copy._username),
							   _realname(copy._realname),
							   _socket(copy._socket),
							   _connected(copy._connected),
							   _logger(copy._logger) {}

ABot::~ABot(void) {}

const string &ABot::getNickname(void) const { return _nickname; }
const string &ABot::getUsername(void) const { return _username; }
const string &ABot::getRealname(void) const { return _realname; }
bool ABot::isConnected(void) const { return _connected; }
int ABot::getSocket(void) const { return _socket; }
void ABot::setLogger(const Logger &logger) { _logger = logger; }
const Logger &ABot::getLogger(void) const { return _logger; }
void ABot::setConnected(bool connected) { _connected = connected; }
void ABot::setSocket(int socket) { _socket = socket; }

void ABot::joinChannel(Channel &channel, const string &key)
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) != _channels.end())
		throw IllegalActionException("Bot is already in channel " + channel_name);

	Message msg(_nickname, JOIN, channel_name.c_str(), key.c_str());
	channel.receiveMessage(msg);
	_channels[channel_name] = &channel;
	_logger.logEvent("Bot joined channel " + channel_name);
}

void ABot::leaveChannel(Channel &channel, const string &reason)
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) == _channels.end())
		throw IllegalActionException("Bot is not in channel " + channel_name);

	const Message msg(_nickname, PART, channel_name.c_str(), reason.c_str());
	channel.receiveMessage(msg);
	_channels.erase(channel_name);
	_logger.logEvent("Bot left channel " + channel_name);
}

void ABot::sendText(const Channel &channel, const string &text)
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) == _channels.end())
		throw IllegalActionException("Bot is not in channel " + channel_name);

	const Message msg(_nickname, PRIVMSG, channel_name.c_str(), text.c_str());
	channel.receiveMessage(msg);
	_logger.logEvent("Bot sent message to channel " + channel_name);
}

void ABot::sendText(const Client &user, const string &text)
{
	const string &user_name = user.getNickname();
	const Message msg(_nickname, PRIVMSG, user_name.c_str(), text.c_str());
	user.receiveMessage(msg);
	_logger.logEvent("Bot sent message to user " + user_name);
}

void ABot::connect(const string &ip, const string &port, const string &password)
{
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo_p(ip.c_str(), port.c_str(), &hints, &res);
	_socket = socket_p(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect_p(_socket, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);

	authenticate(password);
}

void ABot::disconnect(void)
{
	close_p(_socket);
	_socket = -1;
	_connected = false;
	_logger.logEvent("Bot disconnected from server");
}

void ABot::authenticate(const string &password)
{
	const Message pass(_nickname, PASS, password.c_str());
	const Message nick(_nickname, NICK, _nickname.c_str());
	const Message user(_nickname, USER, _username.c_str(), "0", "*", _realname.c_str());

	EventHandler::sendBufferedMessage(_socket, pass);
	EventHandler::sendBufferedMessage(_socket, nick);
	EventHandler::sendBufferedMessage(_socket, user);
}
