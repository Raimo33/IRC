/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 15:36:33 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
#define ABOT_HPP

// TODO capire come gestire l'entrata in canali con password

#include <map>
#include <stdint.h>
#include <string>
#include "CommandMessage.hpp"

#include "Logger.hpp"

class AAction;
class Logger;
class CommandMessage;

class ABot
{
  public:
	explicit ABot(const std::string &nickname, const std::string &username, const std::string &realname);
	ABot(const ABot &copy);
	virtual ~ABot(void) = 0;

	const std::string &getNickname(void) const;
	const std::string &getUsername(void) const;
	const std::string &getRealname(void) const;
	bool               isConnected(void) const;
	int                getSocket(void) const;
	void               setLogger(const Logger &logger);
	const Logger      &getLogger(void) const;

	void run(void);

  protected:
	virtual const std::map<std::string, AAction *> initActions(void) const = 0;

	const std::string                      _nickname;
	const std::string                      _username;
	const std::string                      _realname;
	bool                                   _connected;
	Logger                                 _logger;
	const std::map<std::string, AAction *> _actions;

  private:
	ABot(void);

	void setConnected(bool connected);
	void setSocket(int socket);

	void            connect(void);
	void            authenticate(void);
	void            routine(void);
	void            disconnect(void);
	void            sendMessage(const AMessage &msg) const; // sends a message to the server
	const AMessage *receiveMessage(void) const;             // receives a message from the server
	void            check_for_authentication_success(void) const;

	int         _server_socket; // socket to the
	std::string _server_ip;
	uint16_t    _server_port;
	std::string _server_password;
};

#endif