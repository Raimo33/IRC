/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 18:12:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
#define ABOT_HPP

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
	bool			   isConnected(void) const;
	void			   setLogger(const Logger &logger);
	const Logger	  &getLogger(void) const;
	int				   getSocket(void) const;
	void			   setServerIp(const std::string &ip);
	const std::string &getServerIp(void) const;
	void			   setServerPort(const std::string &port);
	const std::string &getServerPort(void) const;
	void			   setServerPassword(const std::string &password);
	const std::string &getServerPassword(void) const;

	void bindServer(const std::string &ip, const std::string &port, const std::string &password);
	void run(void);

  protected:
	virtual std::map<std::string, AAction *> initActions(void) const = 0;

	const std::string				 _nickname;
	const std::string				 _username;
	const std::string				 _realname;
	bool							 _connected;
	bool							 _authenticated;
	Logger							 _logger;
	std::map<std::string, AAction *> _actions;

  private:
	ABot(void);

	void setConnected(bool connected);
	void setSocket(int socket);

	void			connect(void);
	void			authenticate(void);
	void			routine(void);
	void			handleRequest(void) const;
	void			processInput(const std::string input) const;
	void			disconnect(void);
	void			sendMessage(const AMessage &msg) const;
	const AMessage *receiveMessage(void) const;
	void			check_for_authentication_success(void);

	int			_socket;
	std::string _server_ip;
	std::string _server_port;
	std::string _server_password;
};

#endif