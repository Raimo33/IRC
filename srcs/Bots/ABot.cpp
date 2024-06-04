/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 17:56:48 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"
#include "AAction.hpp"
#include "CommandMessage.hpp"
#include "common_constants.hpp"
#include "common_exceptions.hpp"
#include "common_utils.hpp"
#include "system_calls.hpp"

using std::map;
using std::string;
using std::vector;

ABot::ABot(const string &nickname, const string &username, const string &realname) :
    _nickname(nickname),
    _username(username),
    _realname(realname),
    _connected(false),
    _logger(Logger(nickname)),
    _socket(-1) {}

ABot::ABot(const ABot &copy) :
    _nickname(copy._nickname),
    _username(copy._username),
    _realname(copy._realname),
    _connected(copy._connected),
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
bool          ABot::isConnected(void) const { return _connected; }
void          ABot::setLogger(const Logger &logger) { _logger = logger; }
const Logger &ABot::getLogger(void) const { return _logger; }
int           ABot::getSocket(void) const { return _socket; }
void          ABot::setServerIp(const string &ip) { _server_ip = ip; }
const string &ABot::getServerIp(void) const { return _server_ip; }
void          ABot::setServerPort(uint16_t port) { _server_port = port; }
uint16_t      ABot::getServerPort(void) const { return _server_port; }
void          ABot::setServerPassword(const string &password) { _server_password = password; }
const string &ABot::getServerPassword(void) const { return _server_password; }
void          ABot::setConnected(bool connected) { _connected = connected; }
void          ABot::setSocket(int socket) { _socket = socket; }

void ABot::bindServer(const string &ip, uint16_t port, const string &password)
{
	_server_ip       = ip;
	_server_port     = port;
	_server_password = password;
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
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo_p(_server_ip.c_str(), ::to_string(_server_port).c_str(), &hints, &res);
	_socket = socket_p(res->ai_family, res->ai_socktype, res->ai_protocol);
	configure_non_blocking(_socket);
	connect_p(_socket, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);

	//TODO valutare se mettere select, poll etc anche qua
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

	check_for_authentication_success();
	nick.getDelivered(_socket);
	check_for_authentication_success();
	user.getDelivered(_socket);
	check_for_authentication_success();

	_connected = true;
}

void ABot::routine(void)
{
	while (_connected)
	{
		const AMessage *input  = receiveMessage();
		const AMessage *output = NULL;

		if (dynamic_cast<const ReplyMessage *>(input))
			continue;

		const CommandMessage &command_message = *dynamic_cast<const CommandMessage *>(input);
		const string         &command         = command_message.getParams().front();

		const map<string, AAction *>::const_iterator action_it = _actions.find(command);
		if (action_it == _actions.end())
			output = new ReplyMessage(_nickname, ERR_UNKNOWNCOMMAND, g_default_replies_map.at(ERR_UNKNOWNCOMMAND), NULL);
		else // TODO mettere una ACTION per il comando IVNITE che risponde con JOIN
			output = action_it->second->beExecuted(command_message, *this);
		sendMessage(*output);
		delete input;
		delete output;
	}
}

void ABot::disconnect(void)
{
	close_p(_socket);
	_socket    = -1;
	_connected = false;
	_logger.logEvent("Bot disconnected from server");
}

void ABot::sendMessage(const AMessage &msg) const
{
	msg.getDelivered(_socket);
}

const AMessage *ABot::receiveMessage(void) const
{
	char buffer[BUFFER_SIZE] = { 0 };
	int  ret_val;

	ret_val = recv(_socket, buffer, BUFFER_SIZE - 1, 0);
	if (ret_val == -1)
		return (NULL);
	if (ret_val == 0)
		throw SystemErrorException("Server disconnected");

	CommandMessage tmp(buffer);

	if (tmp.getCommand() == CMD_UNKNOWN)
		return new ReplyMessage(buffer);
	else
		return new CommandMessage(buffer);
}

void ABot::check_for_authentication_success(void) const
{
	const AMessage     *msg   = receiveMessage();
	const ReplyMessage *reply = dynamic_cast<const ReplyMessage *>(msg);
	if (!reply)
		throw InternalErrorException("ABot::check_for_authentication_success: expected reply message");
	if (reply->getReplyCode() != RPL_WELCOME)
		throw SystemErrorException("Authentication failed");
}
