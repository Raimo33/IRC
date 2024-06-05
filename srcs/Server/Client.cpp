/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:45:30 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 17:08:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "server_constants.hpp"
#include "server_exceptions.hpp"
#include "server_utils.hpp"

using std::map;
using std::set;
using std::string;

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
	while (!_channels.empty())
		leaveChannel(*_channels.begin()->second);
}

const map<string, Channel *> &Client::getChannels(void) const
{
	return _channels;
}

void Client::setChannels(const map<string, Channel *> &channels)
{
	_channels = channels;
}

Channel &Client::getChannel(const string &channel_name) const
{
	map<string, Channel *>::const_iterator it = _channels.find(channel_name);

	if (it == _channels.end()) // se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ActionFailedException(ERR_NOTONCHANNEL, channel_name.c_str(), g_default_replies_map.at(ERR_NOTONCHANNEL), NULL);
	return *it->second;
}

void Client::addChannel(Channel &channel)
{
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ActionFailedException(ERR_TOOMANYCHANNELS, channel.getName().c_str(), g_default_replies_map.at(ERR_TOOMANYCHANNELS), NULL);
	_channels[channel.getName()] = &channel;

	ostringstream oss;
	oss << "Client " << _nickname << ", channel added: " << channel.getName();
	_logger.logEvent(oss.str());
}

void Client::removeChannel(Channel &channel)
{
	const string					 channel_name = channel.getName();
	map<string, Channel *>::iterator it = _channels.find(channel_name);

	if (it == _channels.end()) // se client::_channels non ha channel_name vuoldire che il client non è membro di quel canale
		throw ActionFailedException(ERR_NOTONCHANNEL, channel_name.c_str(), g_default_replies_map.at(ERR_NOTONCHANNEL), NULL);
	_channels.erase(it);
	if (channel.getMembers().empty())
	{
		_server->removeChannel(channel);
		delete &channel;
	}

	ostringstream oss;
	oss << "Client " << _nickname << ", channel removed: " << channel_name;
	_logger.logEvent(oss.str());
}

const string &Client::getNickname(void) const
{
	return _nickname;
}

void Client::setNickname(const string &nickname)
{
	_nickname = nickname;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set nickname to " << nickname;
	_logger.logEvent(oss.str());
}

const string &Client::getUsername(void) const
{
	return _username;
}

void Client::setUsername(const string &username)
{
	_username = username;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set username to " << username;
	_logger.logEvent(oss.str());
}

const string &Client::getRealname(void) const
{
	return _realname;
}

void Client::setRealname(const string &realname)
{
	_realname = realname;

	ostringstream oss;
	oss << "Client " << _ip_addr << " has set realname to " << realname;
	_logger.logEvent(oss.str());
}

bool Client::getIsConnected(void) const
{
	return _is_connected;
}

void Client::setIsConnected(bool is_connected)
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

bool Client::getIsAuthenticated(void) const
{
	return _is_authenticated;
}

void Client::setAuthenticated(bool is_authenticated)
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
	const string	   welcome_msg = oss.str();
	const ReplyMessage welcome(SERVER_NAME, RPL_WELCOME, _nickname.c_str(), welcome_msg.c_str(), NULL);
	oss.str("");
	oss << "Your host is " << SERVER_NAME << ", running version " << SERVER_VERSION;
	const string	   host_msg = oss.str();
	const ReplyMessage yourhost(SERVER_NAME, RPL_YOURHOST, _nickname.c_str(), host_msg.c_str(), NULL);
	receiveMessage(&welcome);
	receiveMessage(&yourhost);
}

uint16_t Client::getPort(void) const
{
	return _port;
}

const string &Client::getIpAddr(void) const
{
	return _ip_addr;
}

int Client::getSocket(void) const
{
	return _socket;
}

string Client::getPk(void) const
{
	return _pk;
}

Server &Client::getServer(void) const
{
	return *_server;
}

void Client::joinChannel(Channel &channel, const string &key)
{
	const set<const Client *> &pending_invitations = channel.getPendingInvitations();

	if (!_is_authenticated)
		throw ActionFailedException(ERR_NOTREGISTERED, g_default_replies_map.at(ERR_NOTREGISTERED), NULL);
	if (channel.getMode('i') && pending_invitations.find(this) == pending_invitations.end())
		throw ActionFailedException(ERR_INVITEONLYCHAN, channel.getName().c_str(), g_default_replies_map.at(ERR_INVITEONLYCHAN), NULL);
	if (channel.getMode('k'))
	{
		if (pending_invitations.find(this) == pending_invitations.end() && key != channel.getKey())
			throw ActionFailedException(ERR_BADCHANNELKEY, channel.getName().c_str(), g_default_replies_map.at(ERR_BADCHANNELKEY), NULL);
	}
	if (channel.getMembers().size() >= channel.getMemberLimit())
		throw ActionFailedException(ERR_CHANNELISFULL, channel.getName().c_str(), g_default_replies_map.at(ERR_CHANNELISFULL), NULL);
	if (_channels.size() >= MAX_CHANNELS_PER_USER)
		throw ActionFailedException(ERR_TOOMANYCHANNELS, channel.getName().c_str(), g_default_replies_map.at(ERR_TOOMANYCHANNELS), NULL);

	channel.addMember(*this);
	addChannel(channel);

	const string		&channel_name = channel.getName();
	const string		&channel_topic = channel.getTopic();
	const string		 prefix = _nickname + "!" + _username + "@" + _ip_addr;
	const CommandMessage join_notification(prefix, JOIN, channel_name.c_str(), NULL);
	ReplyMessage		 topic_reply;

	if (channel_topic.empty())
		topic_reply = ReplyMessage(SERVER_NAME, RPL_NOTOPIC, _nickname.c_str(), channel_name.c_str(), g_default_replies_map.at(RPL_NOTOPIC), NULL);
	else
		topic_reply = ReplyMessage(SERVER_NAME, RPL_TOPIC, _nickname.c_str(), channel_name.c_str(), channel_topic.c_str(), NULL);

	const ReplyMessage namreply(SERVER_NAME, RPL_NAMREPLY, _nickname.c_str(), "=", channel_name.c_str(), channel.getMembersString().c_str(), NULL);
	const ReplyMessage endofnames(SERVER_NAME, RPL_ENDOFNAMES, _nickname.c_str(), channel_name.c_str(), g_default_replies_map.at(RPL_ENDOFNAMES), NULL);
	channel.receiveMessage(join_notification);
	receiveMessage(&topic_reply);
	receiveMessage(&namreply);
	receiveMessage(&endofnames);
}

void Client::leaveChannel(Channel &channel, const string &reason)
{
	const string channel_name = channel.getName();
	channel.removeMember(_nickname);

	const CommandMessage part(_nickname.c_str(), PART, channel.getName().c_str(), reason.c_str(), NULL);
	channel.receiveMessage(part);
	receiveMessage(&part);

	removeChannel(channel);

	ostringstream oss;
	oss << "Client " << _nickname << " left channel " << channel_name;
	_logger.logEvent(oss.str());
}

void Client::sendMessage(const Channel &channel, const AMessage &msg) const
{
	const string &channel_name = channel.getName();

	if (_channels.find(channel_name) == _channels.end())
		throw ActionFailedException(ERR_NOTONCHANNEL, channel_name.c_str(), g_default_replies_map.at(ERR_NOTONCHANNEL), NULL);
	channel.receiveMessage(msg, this);

	ostringstream oss;
	oss << "Client " << _nickname << " sent message to channel " << channel_name;
	_logger.logEvent(oss.str());
}

void Client::sendMessage(const Client &receiver, const AMessage &msg) const
{
	if (!receiver.getIsAuthenticated())
		throw ActionFailedException(ERR_NOLOGIN, receiver.getNickname().c_str(), g_default_replies_map.at(ERR_NOLOGIN), NULL);
	receiver.receiveMessage(&msg);

	ostringstream oss;
	oss << "Client " << _nickname << " sent message to client " << receiver.getNickname();
	_logger.logEvent(oss.str());
}

void Client::receiveMessage(const AMessage *msg) const
{
	msg->getDelivered(this->getSocket());
}

void Client::kick(Client &user, Channel &channel, const string &reason) const
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
	const string		 prefix = oss.str();
	const CommandMessage kick_notification(prefix, KICK, channel_name.c_str(), user_nickname.c_str(), reason.c_str(), NULL);
	channel.receiveMessage(kick_notification);
	user.receiveMessage(&kick_notification);
}

void Client::invite(Client &user, Channel &channel) const
{
	checkPrivilege(channel);

	const string &invited_nickname = user.getNickname();
	const string &channel_name = channel.getName();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to invite " << invited_nickname << " to channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.addPendingInvitation(user);
	const ReplyMessage	 reply_to_issuer(SERVER_NAME, RPL_INVITING, _nickname.c_str(), invited_nickname.c_str(), channel_name.c_str(), g_default_replies_map.at(RPL_INVITING), NULL);
	const CommandMessage message_to_target(_nickname, INVITE, _nickname.c_str(), invited_nickname.c_str(), channel_name.c_str(), NULL);
	receiveMessage(&reply_to_issuer);
	user.receiveMessage(&message_to_target);
}

void Client::topicSet(Channel &channel, const string &new_topic) const
{
	const string &channel_name = channel.getName();

	if (channel.getMode('t') && !channel.isOperator(*this))
		throw ActionFailedException(ERR_CHANOPRIVSNEEDED, channel_name.c_str(), g_default_replies_map.at(ERR_CHANOPRIVSNEEDED), NULL);

	ostringstream oss;
	oss << "Client " << _nickname << " tries to set topic of channel " << channel_name << " to " << new_topic;
	_logger.logEvent(oss.str());
	channel.setTopic(new_topic);

	const CommandMessage topic(_nickname, TOPIC, channel_name.c_str(), new_topic.c_str(), NULL);
	channel.receiveMessage(topic);
}

void Client::modeChange(Channel &channel, const char mode, const bool status, const string &param) const
{
	checkPrivilege(channel);

	ostringstream oss;
	oss << "Client " << _nickname << " tries to change mode " << mode << " of channel " << channel.getName();
	_logger.logEvent(oss.str());
	channel.setMode(mode, status, param, this);
}

void Client::modesChange(Channel &channel, const map<char, bool> &modes, const vector<string> &params) const
{
	checkPrivilege(channel);

	uint32_t i = 0;
	for (map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (channel_mode_requires_param(it->first, it->second))
		{
			if (params.empty())
				throw ActionFailedException(ERR_NEEDMOREPARAMS, channel.getName().c_str(), ("Need more parameters for mode " + string(1, it->first)).c_str(), NULL);
			modeChange(channel, it->first, it->second, params.at(i++));
		}
		else
			modeChange(channel, it->first, it->second);
	}
}

void Client::promoteOperator(Channel &channel, Client &user) const
{
	checkPrivilege(channel);
	const string &channel_name = channel.getName();
	const string &user_nickname = user.getNickname();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to promote operator " << user_nickname << " in channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.addOperator(user_nickname);
	const ReplyMessage youreoper(SERVER_NAME, RPL_YOUREOPER, _nickname.c_str(), user_nickname.c_str(), ("You are now an operator of " + channel_name).c_str(), NULL);
	user.receiveMessage(&youreoper);
}

void Client::demoteOperator(Channel &channel, Client &op) const
{
	checkPrivilege(channel);
	const string &channel_name = channel.getName();
	const string &user_nickname = op.getNickname();

	ostringstream oss;
	oss << "Client " << _nickname << " tries to demote operator " << user_nickname << " in channel " << channel_name;
	_logger.logEvent(oss.str());
	channel.removeOperator(op.getNickname());
	const ReplyMessage notoperanymore(SERVER_NAME, RPL_NOTOPERANYMORE, _nickname.c_str(), (user_nickname + " is no longer an operator of " + channel_name).c_str(), NULL);
	op.receiveMessage(&notoperanymore);
}

void Client::checkPrivilege(const Channel &channel) const
{
	if (!channel.isOperator(*this))
		throw ActionFailedException(ERR_CHANOPRIVSNEEDED, channel.getName().c_str(), g_default_replies_map.at(ERR_CHANOPRIVSNEEDED), NULL);
}