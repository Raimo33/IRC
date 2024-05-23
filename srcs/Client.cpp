/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 13:54:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/Message.hpp"
#include "irc/PrivateMessage.hpp"
#include "irc/Server.hpp"
#include "irc/utils.hpp"
#include "irc/Constants.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"

using namespace std;
using namespace irc;

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

	if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ProtocolErrorException(ERR_NOTONCHANNEL, _nickname, channel_name);
	return it->second;
}

void	Client::addChannel(const Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ProtocolErrorException(ERR_TOOMANYCHANNELS, channel.getName());
	_channels[channel.getName()] = &channel;
}

void	Client::removeChannel(const Channel &channel)
{
	const string &channel_name = channel.getName();
	map<string, const Channel *>::iterator it = _channels.find(channel_name);

	if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ProtocolErrorException(ERR_USERNOTINCHANNEL, _nickname, channel_name);
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
			throw InternalErrorException("Client is already connected");
		else
			throw InternalErrorException("Client is already disconnected");
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
			throw InternalErrorException("Client is already authenticated");
		else
			throw InternalErrorException("Client is already unauthenticated");
	}
	_is_authenticated = is_authenticated;
	receiveNumericReply(RPL_WELCOME, vector<string>(1, _nickname), "Welcome to the Internet Relay Network " + _nickname + "!" + _username + "@" + _ip_addr);
	receiveNumericReply(RPL_YOURHOST, vector<string>(1, _nickname), "Your host is " + string(SERVER_NAME) + ", running version " + string(SERVER_VERSION));
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
	try
	{
		channel.addMember(*this); //se fallisce addMember la lascio catchare a chi sta su
		addChannel(channel);
	}
	catch (const ProtocolErrorException &e) //catcho il fallimento di addChannel
	{
		//annullo il successo di addMember
		if (e.getCode() == ERR_TOOMANYCHANNELS)
			channel.removeMember(*this);
		throw e;
	}
}

void	Client::joinChannel(Channel &channel, const string &key)
{
	if (!_is_authenticated)
		throw ProtocolErrorException(ERR_NOTREGISTERED);		
	if (channel.getMode(MODE_K) && channel.getKey() != key)
		throw ProtocolErrorException(ERR_BADCHANNELKEY, channel.getName());
	joinChannel(channel);
	receiveNumericReply(RPL_TOPIC, &vector<string>(1, channel.getName()), channel.getTopic());
	//optional: RPL_TOPIC_WHO_TIME	
	receiveNumericReply(RPL_NAMREPLY, &vector<string>(1, "= " + channel.getName()), channel.getMembersString()); //TODO in futuro mettere channel.getType() al posto di "="
	receiveNumericReply(RPL_ENDOFNAMES, &vector<string>(1, channel.getName()));
	
}

void	Client::leaveChannel(Channel &channel)
{
	if (!_is_authenticated)
		throw ProtocolErrorException(ERR_NOTREGISTERED);
	channel.removeMember(*this);
	removeChannel(channel);
}

void	Client::sendMessage(const Channel &channel, const Message &msg) const
{
	const string &channel_name = channel.getName();
	
	if (_channels.find(channel_name) == _channels.end())
		throw ProtocolErrorException(ERR_NOTONCHANNEL, _nickname, channel_name);
	channel.receiveMessage(msg);
}

void	Client::sendMessage(const Client &receiver, const PrivateMessage &msg) const
{
	if (!receiver.getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOLOGIN, receiver.getNickname());
	EventHandler::sendBufferedMessage(receiver, msg.getContent());
}

// void	Client::receiveNumericReply(uint16_t code, const vector<string> *params, const string &msg) const
// {
// 	struct s_message reply;

// 	reply.prefix = ":" + string(SERVER_NAME);
// 	reply.command = to_string(code);
// 	reply.params.push_back(_nickname);
// 	reply.params.push_back(to_string(code));
// 	reply.params.insert(reply.params.end(), params->begin(), params->end());
	
// 	if (!msg.empty())
// 		reply.params.push_back(msg);
// 	else
// 	{
// 		if (reply_codes.find(code) == reply_codes.end())
// 			InternalErrorException("Client::receiveNumericReply: unknown reply code");
// 		reply.params.push_back(reply_codes.at(code));	
// 	}
// 	EventHandler::sendBufferedMessage(*this, reply);
// }
