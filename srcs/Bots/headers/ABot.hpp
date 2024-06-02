/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 19:02:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
# define ABOT_HPP

#include <string>
#include <map>
#include <stdint.h>

#include "Logger.hpp"

class Action;
class Logger;
class CommandMessage;

class ABot
{
	public:
		explicit ABot(const std::string &nickname, const std::string &username, const std::string &realname);
		ABot(const ABot &copy);
		virtual ~ABot(void) = 0;

		const std::string					&getNickname(void) const;
		const std::string					&getUsername(void) const;
		const std::string					&getRealname(void) const;
		bool								isConnected(void) const;
		int									getSocket(void) const;
		void								setLogger(const Logger &logger);
		const Logger						&getLogger(void) const;

		void								run(void);
	
	protected:

		virtual void						routine(void) = 0;
		void								sendMessage(const CommandMessage &msg) const; //sends a message to the server

		const std::string					_nickname;
		const std::string					_username;
		const std::string					_realname;
		bool								_connected;
		Logger								_logger;
	
	private:
		ABot(void);

		void								setConnected(bool connected);
		void								setSocket(int socket);

		void								connect(void);
		void								disconnect(void);
		void								authenticate(const std::string &password = "");

		int									_server_socket; // socket to the 
		std::string							_server_ip;
		uint16_t							_server_port;
		std::string							_server_password;
		std::map<std::string, Action>		_actions;
};

#endif