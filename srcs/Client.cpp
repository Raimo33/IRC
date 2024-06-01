/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 11:36:22 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Client.hpp"
#include "irc/Channel.hpp"
#include "irc/Server.hpp"
#include "irc/utils.hpp"
#include "irc/Constants.hpp"
#include "irc/Exceptions.hpp"
#include "irc/Message.hpp"

using std::string;
using std::map;

Client::Client(Logger &logger, Server *server, const int socket, const string &ip_addr, const uint16_t port) :
	_is_connected(false),
	_is_authenticated(false),
	_port(port),
	_ip_addr(ip_addr),
	_socket(socket),
	_pk(_ip_addr + ::to_string(_port)),
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
	_server(copy._server),
	_logger(copy._logger) {}

Client::~Client(void)
{
	for (map<string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		it->second->removeMember(_nickname);
}

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
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name.c_str(), default_replies.at(ERR_NOTONCHANNEL), NULL);
	return *it->second;
}

void	Client::addChannel(Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ProtocolErrorException(ERR_TOOMANYCHANNELS, channel.getName().c_str(), default_replies.at(ERR_TOOMANYCHANNELS), NULL);
	_channels[channel.getName()] = &channel;

	ostringstream oss;
	oss << "Client " << _nickname << ", channel added: " << channel.getName();
	_logger.logEvent(oss.str());
}

void	Client::removeChannel(const Channel &channel)
{
	const string &channel_name = channel.getName();
	map<string, Channel *>::iterator it = _channels.find(channel_name);

	if (it == _channels.end()) //se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name.c_str(), default_replies.at(ERR_NOTONCHANNEL), NULL);
	_channels.erase(it);

	ostringstream oss;
	oss << "Client " << _nickname << ", channel removed: " << channel_name;
	_logger.logEvent(oss.str());
}

const string	&Client::getNickname(void) const
{
	return _nickname;
}

void	Client::setNickname(const string &nickname)
{
	_nickname = nickname;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set nickname to " << nickname;
	_logger.logEvent(oss.str());
}

const string	&Client::getUsername(void) const
{
	return _username;
}

void	Client::setUsername(const string &username)
{
	_username = username;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set username to " << username;
	_logger.logEvent(oss.str());
}

const string	&Client::getRealname(void) const
{
	return _realname;
}

void	Client::setRealname(const string &realname)
{
	_realname = realname;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set realname to " << realname;
	_logger.logEvent(oss.str());
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

	ostringstream oss;
	oss << "Client " << _ip_addr << " has " << (is_connected ? "connected" : "disconnected");
	_logger.logEvent(oss.str());
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

	ostringstream oss;
	oss << "Client " << _ip_addr << " is " << (is_authenticated ? "authenticated" : "not authenticated anymore");
	_logger.logEvent(oss.str());

	oss.str("");
	oss << "Welcome to the Internet Relay Network " << _nickname << "!" << _username << "@" << _ip_addr;
	const string welcome_msg = oss.str();
	const Message welcome(SERVER_NAME, RPL_WELCOME, _nickname.c_str(), welcome_msg.c_str(), NULL);
	oss.str("");
	oss << "Your host is " << SERVER_NAME << ", running version " << SERVER_VERSION;
	const string host_msg = oss.str();
	const Message yourhost(SERVER_NAME, RPL_YOURHOST, _nickname.c_str(), host_msg.c_str(), NULL);
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

Server	&Client::getServer(void) const
{
	return *_server;
}

void	Client::joinChannel(Channel &channel, const string &key)
{
	if (!_is_authenticated)
		throw ProtocolErrorException(ERR_NOTREGISTERED, default_replies.at(ERR_NOTREGISTERED), NULL);
	if (channel.getMode('k') && channel.getKey() != key)
		throw ProtocolErrorException(ERR_BADCHANNELKEY, channel.getName().c_str(), default_replies.at(ERR_BADCHANNELKEY), NULL);
	if (channel.getMembers().size() >= channel.getMemberLimit())
		throw ProtocolErrorException(ERR_CHANNELISFULL, channel.getName().c_str(), default_replies.at(ERR_CHANNELISFULL), NULL);
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ProtocolErrorException(ERR_TOOMANYCHANNELS, channel.getName().c_str(), default_replies.at(ERR_TOOMANYCHANNELS), NULL);

	channel.addMember(*this);
	addChannel(channel);

	const string			&channel_name = channel.getName();
	const string			&channel_topic = channel.getTopic();
	const string			prefix = _nickname + "!" + _username + "@" + _ip_addr;
	const Message	join_notification(prefix, JOIN, channel_name.c_str(), NULL);
	Message		topic_reply;

	if (channel_topic.empty())
		topic_reply = Message(SERVER_NAME, RPL_NOTOPIC, _nickname.c_str(), channel_name.c_str(), default_replies.at(RPL_NOTOPIC), NULL);
	else
		topic_reply = Message(SERVER_NAME, RPL_TOPIC, _nickname.c_str(), channel_name.c_str(), channel_topic.c_str(), default_replies.at(RPL_TOPIC), NULL);

	const Message namreply(SERVER_NAME, RPL_NAMREPLY, _nickname.c_str(), "=", channel_name.c_str(), channel.getMembersString().c_str(), NULL);
	const Message endofnames(SERVER_NAME, RPL_ENDOFNAMES, _nickname.c_str(), channel_name.c_str(), default_replies.at(RPL_ENDOFNAMES), NULL);
	channel.receiveMessage(join_notification);
	receiveMessage(topic_reply);
	receiveMessage(namreply);
	receiveMessage(endofnames);
}

void	Client::leaveChannel(Channel &channel, const string &reason)
{
	channel.removeMember(_nickname);
	removeChannel(channel);

	const Message part(_nickname.c_str(), PART, channel.getName().c_str(), reason.c_str(), NULL);
	channel.receiveMessage(part);
	receiveMessage(part);

	ostringstream oss;
	oss << "Client " << _nickname << " left channel " << channel.getName();
	_logger.logEvent(oss.str());
}

void	Client::sendMessage(const Channel &channel, const Message &msg) const
{
	const string &channel_name = channel.getName();
	
	if (_channels.find(channel_name) == _channels.end())
		throw ProtocolErrorException(ERR_NOTONCHANNEL, channel_name.c_str(), default_replies.at(ERR_NOTONCHANNEL), NULL);
	channel.receiveMessage(msg, this);

	ostringstream oss;
	oss << "Client " << _nickname << " sent message to channel " << channel_name;
	_logger.logEvent(oss.str());
}

void	Client::sendMessage(const Client &receiver, const Message &msg) const
{
	if (!receiver.getIsAuthenticated())
		throw ProtocolErrorException(ERR_NOLOGIN, receiver.getNickname().c_str(), default_replies.at(ERR_NOLOGIN), NULL);
	receiver.receiveMessage(msg);

	ostringstream oss;
	oss << "Client " << _nickname << " sent message to client " << receiver.getNickname();
	_logger.logEvent(oss.str());
}

void	Client::receiveMessage(const Message &msg) const
{
	EventHandler::sendBufferedMessage(*this, msg);
}

void	Client::kick(Client &user, Channel &channel, const string &reason) const
{
	checkPrivilege(channel);

	const string &channel_name = channel.getName();
	const string &user_nickname = user.getNickname();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to kick " << user_nickname << " from channel " << channel_name;
	_logger.logEvent(oss.str());
	oss.str("");
	oss << "Client " << _nickname << " tries to kick " << user_nickname << " from channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.removeMember(user_nickname);
	user.removeChannel(channel);

	oss.str("");
	oss << _nickname << "!" << _username << "@" << _ip_addr;
	const string prefix = oss.str();
	const Message kick_notification(prefix, KICK, channel_name.c_str(), user_nickname.c_str(), reason.c_str(), NULL);
	channel.receiveMessage(kick_notification);
	user.receiveMessage(kick_notification);
}

void	Client::invite(Client &user, Channel &channel) const
{
	checkPrivilege(channel);

	const string &nickname = user.getNickname();
	const string &channel_name = channel.getName();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to invite " << nickname << " to channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.addPendingInvitation(user);
	const Message reply_to_issuer(SERVER_NAME, RPL_INVITING, _nickname.c_str(), nickname.c_str(), channel_name.c_str(), default_replies.at(RPL_INVITING), NULL);
	const Message message_to_target(_nickname, INVITE, _nickname.c_str(), nickname.c_str(), channel_name.c_str(), NULL);
	receiveMessage(reply_to_issuer);
	user.receiveMessage(message_to_target);
}

void	Client::topicSet(Channel &channel, const string &new_topic) const
{
	const string &channel_name = channel.getName();

	if (channel.getMode('t') && !channel.isOperator(*this))
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, _nickname.c_str(), channel_name.c_str(), default_replies.at(ERR_CHANOPRIVSNEEDED), NULL);

	ostringstream oss;
	oss << "Client " << _nickname << " tries to set topic of channel " << channel_name << " to " << new_topic;
	_logger.logEvent(oss.str());
	channel.setTopic(new_topic);

	const Message topic(_nickname, TOPIC, channel_name.c_str(), new_topic.c_str(), NULL);
	channel.receiveMessage(topic);
}

void	Client::modeChange(Channel &channel, const char mode, const bool status, const string &param) const
{
	checkPrivilege(channel);

	ostringstream oss;
	oss << "Client " << _nickname << " tries to change mode " << mode << " of channel " << channel.getName();
	_logger.logEvent(oss.str());
	channel.setMode(mode, status, param, this);
}

void	Client::modesChange(Channel &channel, const map<char, bool> &modes, const vector<string> &params) const
{
	checkPrivilege(channel);

	uint32_t i = 0;
	for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (channel_mode_requires_param(it->first, it->second))
		{
			if (params.empty())
				throw ProtocolErrorException(ERR_NEEDMOREPARAMS, channel.getName().c_str(), ("Need more parameters for mode " + string(1, it->first)).c_str(), NULL);
			modeChange(channel, it->first, it->second, params.at(i++));
		}
		else
			modeChange(channel, it->first, it->second);
	}
}

void	Client::promoteOperator(Channel &channel, Client &user)
{
	checkPrivilege(channel);
	const string &channel_name = channel.getName();
	const string &user_nickname = user.getNickname();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to promote operator " << user_nickname << " in channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.addOperator(user_nickname);
	const Message youreoper(SERVER_NAME, RPL_YOUREOPER, _nickname.c_str(), user_nickname.c_str(), ("You are now an operator of " + channel_name).c_str(), NULL);
	user.receiveMessage(youreoper);
}

void	Client::demoteOperator(Channel &channel, Client &op)
{
	checkPrivilege(channel);
	const string &channel_name = channel.getName();
	const string &user_nickname = op.getNickname();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to demote operator " << user_nickname << " in channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.removeOperator(op.getNickname());
	const Message notoperanymore(SERVER_NAME, RPL_NOTOPERANYMORE, _nickname.c_str(), (user_nickname + " is no longer an operator of " + channel_name).c_str(), NULL);
	op.receiveMessage(notoperanymore);
}

void	Client::checkPrivilege(const Channel &channel) const
{
	if (!channel.isOperator(*this))
		throw ProtocolErrorException(ERR_CHANOPRIVSNEEDED, channel.getName().c_str(), default_replies.at(ERR_CHANOPRIVSNEEDED), NULL);
}