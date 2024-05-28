/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 16:42:53 by craimond         ###   ########.fr       */
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

Client::Client(Logger &logger, Server *server, const int socket, const pollfd &pollfd, const string &ip_addr, const uint16_t port) :
	_channels(),
	_nickname(),
	_username(),
	_realname(),
	_is_connected(false),
	_is_authenticated(false),
	_port(port),
	_ip_addr(ip_addr),
	_socket(socket),
	_pk(_ip_addr + ::to_string(_port)),
	_pollfd(pollfd),
	_server(server),
	_logger(logger) {}

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
	_pk(copy._pk),
	_pollfd(copy._pollfd),
	_server(copy._server),
	_logger(copy._logger) {}

Client::~Client(void) {}

const map<string, Channel *>	&Client::getChannels(void) const
{
	return _channels;
}

void	Client::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

Channel	&Client::getChannel(const string &channel_name) const
{
	map<string, Channel *>::const_iterator it = _channels.find(channel_name);

	if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
	return *it->second;
}

void	Client::addChannel(Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ProtocolErrorException(ERR_TOOMANYCHANNELS, channel.getName());
	_channels[channel.getName()] = &channel;
	_logger.logEvent("Client " + _nickname + ", channel added: " + channel.getName());
}

void	Client::removeChannel(const Channel &channel)
{
	const string &channel_name = channel.getName();
	map<string, Channel *>::iterator it = _channels.find(channel_name);

	if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
	_channels.erase(it);
	_logger.logEvent("Client " + _nickname + ", channel removed: " + channel_name);
}

const string	&Client::getNickname(void) const
{
	return _nickname;
}

void	Client::setNickname(const string &nickname)
{
	_nickname = nickname;
	_logger.logEvent("Client " + _ip_addr + " has set nickname to " + nickname);
}

const string	&Client::getUsername(void) const
{
	return _username;
}

void	Client::setUsername(const string &username)
{
	_username = username;
	_logger.logEvent("Client " + _ip_addr + " has set username to " + username);
}

const string	&Client::getRealname(void) const
{
	return _realname;
}

void	Client::setRealname(const string &realname)
{
	_realname = realname;
	_logger.logEvent("Client " + _ip_addr + " has set realname to " + realname);
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
	_logger.logEvent("Client " + _ip_addr + " has " + (is_connected ? "connected" : "disconnected"));
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
	_logger.logEvent("Client " + _ip_addr + " is " + (is_authenticated ? "authenticated" : "not authenticated anymore"));

	const struct s_replyMessage welcome = EventHandler::buildReplyMessage(RPL_WELCOME, "", "Welcome to the Internet Relay Network " + _nickname + "!" + _username + "@" + _ip_addr);
	const struct s_replyMessage yourhost = EventHandler::buildReplyMessage(RPL_YOURHOST, "", "Your host is " + string(SERVER_NAME) + ", running version " + SERVER_VERSION);
	receiveMessage(welcome);
	receiveMessage(yourhost);
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

string	Client::getPk(void) const
{
	return _pk;
}

const pollfd	&Client::getPollfd(void) const
{
	return _pollfd;
}

Server	&Client::getServer(void) const
{
	return *_server;
}

void	Client::joinChannel(Channel &channel, const string &key)
{
	if (!_is_authenticated)
		throw ProtocolErrorException(ERR_NOTREGISTERED);
	if (channel.getMode('k') && channel.getKey() != key)
		throw ProtocolErrorException(ERR_BADCHANNELKEY, channel.getName());
	if (channel.getMembers().size() >= channel.getMemberLimit())
		throw ProtocolErrorException(ERR_CHANNELISFULL, channel.getName());
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ProtocolErrorException(ERR_TOOMANYCHANNELS, channel.getName());

	channel.addMember(*this);
	addChannel(channel);

	const string					&channel_name = channel.getName();
	const string					&channel_topic = channel.getTopic();
	const struct s_commandMessage	join_acknowledgement = EventHandler::buildCommandMessage(_nickname, JOIN, channel_name);
	struct s_replyMessage			topic_reply;

	if (channel_topic.empty())
		topic_reply = EventHandler::buildReplyMessage(RPL_NOTOPIC, channel_name);
	else
		topic_reply = EventHandler::buildReplyMessage(RPL_TOPIC, channel_name, channel_topic);

	vector<string> params;
	params.push_back("=");
	params.push_back(channel_name);
	const struct s_replyMessage namreply = EventHandler::buildReplyMessage(RPL_NAMREPLY, params, channel.getMembersString());
	const struct s_replyMessage endofnames = EventHandler::buildReplyMessage(RPL_ENDOFNAMES, channel_name);
	receiveMessage(join_acknowledgement);
	receiveMessage(topic_reply);
	receiveMessage(namreply);
	receiveMessage(endofnames);
}

void	Client::leaveChannel(Channel &channel, const string &reason)
{
	channel.removeMember(*this);
	removeChannel(channel);

	const struct s_commandMessage part = EventHandler::buildCommandMessage(_nickname, PART, channel.getName(), reason);
	channel.receiveMessage(part);
}

void	Client::sendMessage(const Channel &channel, const struct s_commandMessage &msg) const
{
	const string &channel_name = channel.getName();
	
	if (_channels.find(channel_name) == _channels.end())
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name);
	channel.receiveMessage(msg);
	_logger.logEvent("Client " + _nickname + " sent message to channel " + channel_name);
}

void	Client::sendMessage(const Client &receiver, const struct s_commandMessage &msg) const
{
	if (!receiver.getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOLOGIN, receiver.getNickname());
	receiver.receiveMessage(msg);
	_logger.logEvent("Client " + _nickname + " sent message to client " + receiver.getNickname());
}

void	Client::receiveMessage(const struct s_messageBase &msg) const
{
	EventHandler::sendBufferedContent(*this, &msg);
}

void	Client::kick(Client &user, Channel &channel, const string &reason) const
{
	checkPrivilege(channel);

	const string &channel_name = channel.getName();

	_logger.logEvent("Client " + _nickname + " tries to kick " + user.getNickname() + " from channel " + channel_name);
	channel.removeMember(user);
	user.removeChannel(channel);

	vector<string> params;
	params.push_back(channel_name);
	params.push_back(_nickname);
	const struct s_commandMessage message_to_channel = EventHandler::buildCommandMessage("", KICK, params, reason);
	channel.receiveMessage(message_to_channel);
}

void	Client::invite(Client &user, Channel &channel) const
{
	checkPrivilege(channel);

	const string &nickname = user.getNickname();
	const string &channel_name = channel.getName();

	_logger.logEvent("Client " + _nickname + " tries to invite " + user.getNickname() + " to channel " + channel.getName());
	channel.addPendingInvitation(user);

	vector<string> params;
	params.push_back(nickname);
	params.push_back(channel_name);
	channel.addPendingInvitation(user);
	{
		const struct s_replyMessage reply_to_issuer = EventHandler::buildReplyMessage(RPL_INVITING, params);
		receiveMessage(reply_to_issuer);
	}
	{
		const struct s_commandMessage message_to_target = EventHandler::buildCommandMessage(_nickname, INVITE, params);
		user.receiveMessage(message_to_target);
	}
}

void	Client::topicSet(Channel &channel, const string &new_topic) const
{
	if (channel.getMode('t') && !channel.isOperator(*this))
	{
		vector<string> params;
		params.push_back(_nickname);
		params.push_back(channel.getName());
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, params);	
	}
	_logger.logEvent("Client " + _nickname + " tries to set topic of channel " + channel.getName() + " to " + new_topic);
	channel.setTopic(new_topic);

	const struct s_commandMessage topic = EventHandler::buildCommandMessage(_nickname, TOPIC, channel.getName(), new_topic);
	channel.receiveMessage(topic);
}

void	Client::modeChange(Channel &channel, const char mode, const bool status, const string &param) const
{
	checkPrivilege(channel);
	_logger.logEvent("Client " + _nickname + " tries to change mode of channel " + channel.getName() + " to " + mode + (status ? "+" : "-") + param);
	channel.setMode(mode, status, param);
}

void	Client::modesChange(Channel &channel, const vector<bool> &modes, const vector<string> &params) const
{
	checkPrivilege(channel);
	_logger.logEvent("Client " + _nickname + " tries to change modes of channel " + channel.getName());
	channel.setModes(modes, params);
}

void	Client::promoteOperator(Channel &channel, Client &user)
{
	checkPrivilege(channel);
	_logger.logEvent("Client " + _nickname + " tries to promote operator " + user.getNickname() + " in channel " + channel.getName());
	channel.addOperator(user);
}

void	Client::demoteOperator(Channel &channel, Client &op)
{
	checkPrivilege(channel);
	_logger.logEvent("Client " + _nickname + " tries to demote operator " + op.getNickname() + " in channel " + channel.getName());
	channel.removeOperator(op);
}

void	Client::checkPrivilege(const Channel &channel) const
{
	if (!channel.isOperator(*this))
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, channel.getName());
}