/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/07/03 18:05:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"
#include "AAction.hpp"
#include "CommandMessage.hpp"
#include "bot_constants.hpp"
#include "bot_exceptions.hpp"
#include "bot_utils.hpp"
#include "system_calls.hpp"
#include "SignalHandler.hpp"

using std::map;
using std::string;
using std::vector;

ABot::ABot(const string &nickname, const string &username, const string &realname) :
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_connected(false),
	_authenticated(false),
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
	if (_connected)
		disconnect();
	for (map<string, AAction *>::const_iterator it = _actions.begin(); it != _actions.end(); ++it)
		delete it->second;
	_logger.logEvent("Bot shutting down");
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
	stop();
}

void ABot::stop(void)
{
	ABot::~ABot();
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

	while (g_received_signal != SIGINT)
	{
		poll_p(&pfd, 1, -1);

		if (pfd.revents & POLLIN)
			handleRequest();
		if (pfd.revents & (POLLHUP | POLLERR))
			return;
	}
}

void ABot::processInput(const string raw_input) const
{
	vector<string> inputs = ::split(raw_input, "\r\n");
	int			   n_inputs = inputs.size();

	for (int i = 0; i < n_inputs; i++)
	{
		if (inputs[i].empty())
			continue;

		_logger.logEvent("Message received: " + inputs[i]);

		CommandMessage input(inputs[i]);
		if (input.getCommand() == CMD_UNKNOWN)
			continue;

		const AMessage *output;
		if (input.getCommand() == INVITE)
		{
			const string &channel_name = input.getParams().at(2);
			output = new CommandMessage(_nickname, JOIN, channel_name.c_str(), NULL);
		}
		else
		{
			const string								&action = input.getParams().back();
			const map<string, AAction *>::const_iterator action_it = _actions.find(action);
			if (action_it == _actions.end())
				continue;
			output = action_it->second->beExecuted(input, *this);
		}
		sendMessage(*output);
	}
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

void ABot::handleRequest(void) const
{
	static string full_buffer;
	char		  buffer[BUFFER_SIZE] = { 0 };

	if (recv_p(_socket, buffer, sizeof(buffer) - 1, 0) == 0)
		throw SystemErrorException("Server disconnected");
	full_buffer += buffer;
	if (full_buffer.find("\r\n") == string::npos)
		return;

	processInput(full_buffer);
	full_buffer.clear();
}
