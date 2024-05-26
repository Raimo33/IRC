/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 17:14:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/Server.hpp"
#include "irc/utils.hpp"
#include "irc/Constants.hpp"
#include "irc/Exceptions.hpp"
#include "irc/ReplyCodes.hpp"

using std::string;
using std::map;

namespace irc
{
	Client::Client(Server *server, const int socket, const string &ip_addr, const uint16_t port) :
		_channels(),
		_nickname(),
		_username(),
		_realname(),
		_is_connected(false),
		_is_authenticated(false),
		_port(port),
		_ip_addr(ip_addr),
		_socket(socket),
		_server(server) {}

	Client::Client(const Client &copy) :
		_channels(copy._channels),
		_nickname(copy._nickname),
		_username(copy._username),
		_realname(copy._realname),
		_is_connected(copy._is_connected),
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

	const Channel	&Client::getChannel(const string &channel_name) const
	{
		map<string, const Channel *>::const_iterator it = _channels.find(channel_name);

		if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
			throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
		return *it->second;
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
			throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
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

	const string	&Client::getRealname(void) const
	{
		return _realname;
	}

	void	Client::setRealname(const string &realname)
	{
		_realname = realname;
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

		{
			const struct s_replyContent welcome = EventHandler::buildReplyContent(RPL_WELCOME, NULL, "Welcome to the Internet Relay Network" + _nickname + "!" + _username + "@" + _ip_addr);
			EventHandler::sendBufferedContent(*this, &welcome);
		}
		{
			const struct s_replyContent yourhost = EventHandler::buildReplyContent(RPL_YOURHOST, NULL, "Your host is " + string(SERVER_NAME) + ", running version " + SERVER_VERSION);
			EventHandler::sendBufferedContent(*this, &yourhost);
		}
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

	Server	&Client::getServer(void) const
	{
		return *_server;
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
			if (e.getContent().code == ERR_TOOMANYCHANNELS)
				channel.removeMember(*this);
			throw e;
		}
	}

	void	Client::joinChannel(Channel &channel, const string &key)
	{
		if (!_is_authenticated)
			throw ProtocolErrorException(ERR_NOTREGISTERED);
		if (channel.getMode('k') && channel.getKey() != key)
			throw ProtocolErrorException(ERR_BADCHANNELKEY, channel.getName());
		joinChannel(channel);

		//TODO in futuro mettere channel.getType() al posto di "="
		const struct s_replyContent topic = EventHandler::buildReplyContent(RPL_TOPIC, channel.getName(), channel.getTopic()); // TODO valutare se mettere qualcosa quando ik topic [ vuoto ]
		const string params[] = { "=", channel.getName() };
		const struct s_replyContent namreply = EventHandler::buildReplyContent(RPL_NAMREPLY, params, channel.getMembersString());
		const struct s_replyContent endofnames = EventHandler::buildReplyContent(RPL_ENDOFNAMES, channel.getName());
		EventHandler::sendBufferedContent(*this, &topic);
		EventHandler::sendBufferedContent(*this, &namreply);
		EventHandler::sendBufferedContent(*this, &endofnames);
	}

	void	Client::leaveChannel(Channel &channel)
	{
		if (!_is_authenticated)
			throw ProtocolErrorException(ERR_NOTREGISTERED);
		channel.removeMember(*this);
		removeChannel(channel);
	}

	void	Client::sendMessage(const Channel &channel, const struct s_commandContent &msg) const
	{
		const string &channel_name = channel.getName();
		
		if (_channels.find(channel_name) == _channels.end())
			throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
		channel.receiveMessage(msg);
	}

	void	Client::sendMessage(const Client &receiver, const struct s_commandContent &msg) const
	{
		if (!receiver.getIsAuthenticated())
			throw ProtocolErrorException(ERR_NOLOGIN, receiver.getNickname());
		EventHandler::sendBufferedContent(receiver, &msg);
	}
}