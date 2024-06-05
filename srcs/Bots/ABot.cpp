/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 13:30:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"
#include "AAction.hpp"
#include "CommandMessage.hpp"
#include "bot_constants.hpp"
#include "bot_exceptions.hpp"
#include "bot_utils.hpp"
#include "system_calls.hpp"

using std::map;
using std::string;
using std::vector;

ABot::ABot(const string &nickname, const string &username, const string &realname) :
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_connected(false),
	_authenticated(false),
	_logger(Logger(nickname)),
	_socket(-1) {}

ABot::ABot(const ABot &copy) :
	_nickname(copy._nickname),
	_username(copy._username),
	_realname(copy._realname),
	_connected(copy._connected),
	_authenticated(copy._authenticated),
	_logger(copy._logger),
	_actions(copy._actions),
	_socket(copy._socket),
	_server_ip(copy._server_ip),
	_server_port(copy._server_port),
	_server_password(copy._server_password) {}

ABot::~ABot(void)
{
	for (map<string, AAction *>::const_iterator it = _actions.begin(); it != _actions.end(); ++it)
		delete it->second;
}

const string &ABot::getNickname(void) const { return _nickname; }
const string &ABot::getUsername(void) const { return _username; }
const string &ABot::getRealname(void) const { return _realname; }
bool		  ABot::isConnected(void) const { return _connected; }
void		  ABot::setLogger(const Logger &logger) { _logger = logger; }
const Logger &ABot::getLogger(void) const { return _logger; }
int			  ABot::getSocket(void) const { return _socket; }
void		  ABot::setServerIp(const string &ip) { _server_ip = ip; }
const string &ABot::getServerIp(void) const { return _server_ip; }
void		  ABot::setServerPort(const string &port) { _server_port = port; }
const string &ABot::getServerPort(void) const { return _server_port; }
void		  ABot::setServerPassword(const string &password) { _server_password = password; }
const string &ABot::getServerPassword(void) const { return _server_password; }
void		  ABot::setConnected(bool connected) { _connected = connected; }
void		  ABot::setSocket(int socket) { _socket = socket; }

void ABot::bindServer(const string &ip, const string &port, const string &password)
{
	setServerIp(ip);
	setServerPort(port);
	setServerPassword(password);
}

void ABot::run(void)
{
	connect();
	authenticate();
	routine();
	disconnect();
}

void ABot::connect(void)
{
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo_p(_server_ip.c_str(), _server_port.c_str(), &hints, &res);
	_socket = socket_p(res->ai_family, res->ai_socktype, res->ai_protocol);
	configure_non_blocking(_socket);
	if (::connect(_socket, res->ai_addr, res->ai_addrlen) == -1 && errno != EINPROGRESS)
		throw SystemErrorException(errno);
	else if (errno == EINPROGRESS)
	{
		struct pollfd pfd;
		pfd.fd = _socket;
		pfd.events = POLLOUT;
		poll_p(&pfd, 1, -1);
		socklen_t len = sizeof(pfd.revents);
		getsockopt_p(_socket, SOL_SOCKET, SO_ERROR, &pfd.revents, &len);
		if (pfd.revents & (POLLHUP | POLLERR))
			throw SystemErrorException("Connection failed");
	}
	freeaddrinfo(res);

	_logger.logEvent("Bot connected to server");
	_connected = true;
}

void ABot::authenticate(void)
{
	const CommandMessage nick(_nickname, NICK, _nickname.c_str(), NULL);
	const CommandMessage user(_nickname, USER, _username.c_str(), "0", "*", _realname.c_str(), NULL);

	if (!_server_password.empty())
	{
		const CommandMessage pass(_nickname, PASS, _server_password.c_str(), NULL);
		pass.getDelivered(_socket);
	}

	nick.getDelivered(_socket);
	user.getDelivered(_socket);

	_logger.logEvent("Bot authenticated successfully");
	_authenticated = true;
}

void ABot::routine(void)
{
	struct pollfd pfd;

	pfd.fd = _socket;
	pfd.events = POLLIN | POLLHUP | POLLERR;

	while (_connected)
	{
		poll_p(&pfd, 1, -1);

		if (pfd.revents & POLLIN)
			handleRequest();
		if (pfd.revents & (POLLHUP | POLLERR))
			return;
	}
}

void ABot::handleRequest(void)
{
	const AMessage		 *input = receiveMessage();
	const AMessage		 *output = NULL;
	const CommandMessage *input_message = NULL;

	if (dynamic_cast<const ReplyMessage *>(input))
		goto cleanup;

	input_message = dynamic_cast<const CommandMessage *>(input);
	if (input_message->getCommand() == INVITE)
	{
		const string &channel_name = input_message->getParams().at(2);
		output = new CommandMessage(_nickname, JOIN, channel_name.c_str(), NULL);
	}
	else
	{
		const string								&action = input_message->getParams().front();
		const map<string, AAction *>::const_iterator action_it = _actions.find(action);
		if (action_it == _actions.end())
			goto cleanup;
		output = action_it->second->beExecuted(*input_message, *this);
	}

	sendMessage(*output);
cleanup:
	delete input;
	delete output;
}

void ABot::disconnect(void)
{
	close_p(_socket);
	_socket = -1;
	_connected = false;
	_logger.logEvent("Bot disconnected from server");
}

void ABot::sendMessage(const AMessage &msg) const
{
	msg.getDelivered(_socket);
	ostringstream oss;
	oss << "Message sent: " << msg.getRaw();
	_logger.logEvent(oss.str());
}

const AMessage *ABot::receiveMessage(void) const
{
	static string full_buffer;
	char		  buffer[BUFFER_SIZE] = { 0 };

	if (recv_p(_socket, buffer, BUFFER_SIZE - 1, 0) == 0)
		throw SystemErrorException("Server disconnected");

	full_buffer += buffer;
	if (full_buffer.find("\r\n") == string::npos)
		return NULL;

	AMessage	  *ret = NULL;
	CommandMessage tmp(full_buffer);

	ostringstream oss;
	oss << "Message received: " << tmp.getRaw();
	_logger.logEvent(oss.str());

	if (tmp.getCommand() == CMD_UNKNOWN)
		ret = new ReplyMessage(full_buffer);
	else
		ret = new CommandMessage(tmp);
	full_buffer.clear();
	return ret;
}
