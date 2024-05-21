/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 15:52:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"
#include "headers/Channel.hpp"
#include "headers/Message.hpp"
#include "headers/PrivateMessage.hpp"
#include "headers/Server.hpp"
#include "headers/utils.hpp"
#include "headers/Standards.hpp"
#include "headers/IRC_Exceptions.hpp"

Client::Client(Server *server, const int socket, const string &ip_addr, const uint16_t port) :
	_channels(),
	_nickname(),
	_username(),
	_is_authenticated(false),
	_port(port),
	_ip_addr(ip_addr),
	_socket(socket),
	_server(server) {}

Client::Client(const Client &copy) :
	_channels(copy._channels),
	_nickname(copy._nickname),
	_username(copy._username),
	_is_authenticated(copy._is_authenticated),
	_port(copy._port),
	_ip_addr(copy._ip_addr),
	_socket(copy._socket),
	_server(copy._server) {}

Client::~Client(void) {}

const map<string, const Channel *>	&Client::getChannels(void) const
{
	return _channels;
}

void	Client::setChannels(const map<string, const Channel *> &channels)
{
	_channels = channels;
}

const Channel	*Client::getChannel(const string &channel_name) const
{
	map<string, const Channel *>::const_iterator it = _channels.find(channel_name);

	if (it == _channels.end())
		throw UserNotInChannelException();
	return it->second;
}

void	Client::addChannel(const Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw TooManyChannelsException();
	_channels[channel.getName()] = &channel;
}

void	Client::removeChannel(const Channel &channel)
{
	map<string, const Channel *>::iterator it = _channels.find(channel.getName());

	if (it == _channels.end())
		throw UserNotInChannelException();
	_channels.erase(it);
}

const string	&Client::getNickname(void) const
{
	return _nickname;
}

void	Client::setNickname(const string &nickname)
{
	_nickname = nickname;
}

const string	&Client::getUsername(void) const
{
	return _username;
}

void	Client::setUsername(const string &username)
{
	_username = username;
}

bool	Client::getIsConnected(void) const
{
	return _is_connected;
}

void	Client::setIsConnected(bool is_connected)
{
	if (_is_connected == is_connected)
	{
		if (is_connected)
			throw AlreadyConnectedException();
		else
			throw AlreadyConnectedException();
	}
	_is_connected = is_connected;
}

bool	Client::getIsAuthenticated(void) const
{
	return _is_authenticated;
}

void	Client::setAuthenticated(bool is_authenticated)
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

uint16_t	Client::getPort(void) const
{
	return _port;
}

const string	&Client::getIpAddr(void) const
{
	return _ip_addr;
}

int	Client::getSocket(void) const
{
	return _socket;
}

Server	*Client::getServer(void) const
{
	return _server;
}

void	Client::joinChannel(Channel &channel)
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

void	Client::joinChannel(Channel &channel, const string &key)
{
	if (!_is_authenticated)
		throw NotAuthenticatedException();
	if (channel.getKey() != key)
		throw Channel::InvalidKeyException();
	joinChannel(channel);
}

void	Client::leaveChannel(Channel &channel)
{
	if (!_is_authenticated)
		throw NotAuthenticatedException();
	channel.removeMember(*this);
	removeChannel(channel);
}

void	Client::sendMessage(const Channel &channel, const Message &msg) const
{
	if (_channels.find(channel.getName()) == _channels.end())
		throw UserNotInChannelException();
	if (channel.getMembers().size() == 1)
		throw CantSendMessageToYourselfException();
	channel.receiveMessage(msg);
}

void	Client::sendMessage(const Client &receiver, const PrivateMessage &msg) const
{
	if (receiver.getUsername() == _username)
		throw CantSendMessageToYourselfException();
	EventHandler::sendBufferedString(receiver, msg.getContent());
}

void	Client::receiveNumericReply(uint16_t code, const vector<string> &params, const string &msg) const
{
	ostringstream oss;

	oss << ":" SERVER_NAME << " " << code << " " << _nickname;
	for (size_t i = 0; i < params.size(); i++)
		oss << " " << params[i];
	if (!msg.empty())
		oss << " :" << msg;
	else
	{
		if (Server::reply_codes.find(code) == Server::reply_codes.end())
			Server::FatalErrorException("Internal error: unknown reply code");
		oss << " :" << Server::reply_codes.at(code);	
	}
	EventHandler::sendBufferedString(*this, oss.str());
}
