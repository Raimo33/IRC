/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:30:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 17:08:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABot.hpp"
#include "CommandMessage.hpp"
#include "system_calls.hpp"
#include "common_utils.hpp"

using std::string;

ABot::ABot(const string &nickname, const string &username, const string &realname) :
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_connected(false),
	_logger(Logger(nickname)),
	_server_socket(-1) {}

ABot::ABot(const ABot &copy) :
	_nickname(copy._nickname),
	_username(copy._username),
	_realname(copy._realname),
	_connected(copy._connected),
	_logger(copy._logger),
	_server_socket(copy._server_socket),
	_actions(copy._actions) {}

ABot::~ABot(void) {}

const string &ABot::getNickname(void) const { return _nickname; }
const string &ABot::getUsername(void) const { return _username; }
const string &ABot::getRealname(void) const { return _realname; }
bool ABot::isConnected(void) const { return _connected; }
int ABot::getSocket(void) const { return _server_socket; }
void ABot::setLogger(const Logger &logger) { _logger = logger; }
const Logger &ABot::getLogger(void) const { return _logger; }
void ABot::setConnected(bool connected) { _connected = connected; }
void ABot::setSocket(int socket) { _server_socket = socket; }

void	ABot::run(void)
{
	connect();
	authenticate();
	routine();
	disconnect();
}

void	ABot::connect(void)
{
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo_p(_server_ip.c_str(), ::to_string(_server_port).c_str(), &hints, &res);
	_server_socket = socket_p(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect_p(_server_socket, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
}

void	ABot::disconnect(void)
{
	close_p(_server_socket);
	_server_socket = -1;
	_connected = false;
	_logger.logEvent("Bot disconnected from server");
}

void	ABot::sendMessage(const CommandMessage &msg) const
{
	msg.getDelivered(_server_socket);
}

void	ABot::authenticate(void)
{
	const CommandMessage nick(_nickname, NICK, _nickname.c_str(), NULL);
	const CommandMessage user(_nickname, USER, _username.c_str(), "0", "*", _realname.c_str(), NULL);

	if (!_server_password.empty())
	{
		const CommandMessage pass(_nickname, PASS, _server_password.c_str(), NULL);
		pass.getDelivered(_server_socket);
	}

	nick.getDelivered(_server_socket);
	user.getDelivered(_server_socket);

	_connected = true;
}
