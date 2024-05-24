/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 17:10:59 by craimond         ###   ########.fr       */
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

using std::string;
using std::map;

namespace irc
{
	Client::Client(Server *server, const int socket, const string &ip_addr, const uint16_t port) :
		_channels(),
		_nickname(),
		_username(),
		_realname(),
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
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_NOTONCHANNEL, channel_name.c_str()));
		return *it->second;
	}

	void	Client::addChannel(const Channel &channel)
	{
		if (_channels.size() >= MAX_CHANNELS_PER_USER)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_TOOMANYCHANNELS, channel.getName().c_str()));
		_channels[channel.getName()] = &channel;
	}

	void	Client::removeChannel(const Channel &channel)
	{
		const string &channel_name = channel.getName();
		map<string, const Channel *>::iterator it = _channels.find(channel_name);

		if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_USERNOTINCHANNEL, _nickname.c_str(), channel_name.c_str()));
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

		const struct s_replyContent welcome = EventHandler::buildReplyContent(("Welcome to the Internet Relay Network " + _nickname + "!" + _username + "@" + _ip_addr).c_str(), RPL_WELCOME, _nickname.c_str());
		const struct s_replyContent yourhost = EventHandler::buildReplyContent("Your host is " + string(SERVER_NAME) + ", running version " + SERVER_VERSION, RPL_YOURHOST, _nickname.c_str());
		EventHandler::sendBufferedContent(*this, &welcome);
		EventHandler::sendBufferedContent(*this, &yourhost);
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
		if (_is_authenticated == false)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_NOTREGISTERED));		
		if (channel.getMode(MODE_K) && channel.getKey() != key)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_BADCHANNELKEY, channel.getName().c_str()));
		joinChannel(channel);

		const struct s_replyContent topic = EventHandler::buildReplyContent(channel.getTopic(), RPL_TOPIC, channel.getName().c_str());
		const struct s_replyContent namreply = EventHandler::buildReplyContent(channel.getMembersString(), RPL_NAMREPLY, "=", channel.getName().c_str());
		const struct s_replyContent endofnames = EventHandler::buildReplyContent("", RPL_ENDOFNAMES, channel.getName().c_str());
		//TODO in futuro mettere channel.getType() al posto di "="
		EventHandler::sendBufferedContent(*this, &topic);
		EventHandler::sendBufferedContent(*this, &namreply);
		EventHandler::sendBufferedContent(*this, &endofnames);	
	}

	void	Client::leaveChannel(Channel &channel)
	{
		if (_is_authenticated == false)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_NOTREGISTERED));
		channel.removeMember(*this);
		removeChannel(channel);
	}

	void	Client::sendMessage(const Channel &channel, const Message &msg) const
	{
		const string &channel_name = channel.getName();
		
		if (_channels.find(channel_name) == _channels.end())
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_NOTONCHANNEL, channel_name.c_str()));
		channel.receiveMessage(msg);
	}

	void	Client::sendMessage(const Client &receiver, const PrivateMessage &msg) const
	{
		if (receiver.getIsAuthenticated() == false)
			throw ProtocolErrorException(EventHandler::buildReplyContent("", ERR_NOLOGIN, receiver.getNickname().c_str()));

		const struct s_commandContent msg_content = EventHandler::buildCommandContent(_nickname, msg.getText(), PRIVMSG, receiver.getNickname().c_str());
		EventHandler::sendBufferedContent(receiver, &msg_content);
	}
}