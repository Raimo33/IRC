/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 18:19:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
# define ABOT_HPP

//TODO capire come gestire l'entrata in canali con password

#include <string>
#include <map>
#include <stdint.h>
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

		const std::string						&getNickname(void) const;
		const std::string						&getUsername(void) const;
		const std::string						&getRealname(void) const;
		bool									isConnected(void) const;
		int										getSocket(void) const;
		void									setLogger(const Logger &logger);
		const Logger							&getLogger(void) const;

		void									run(void);
	
	protected:


		const std::string						_nickname;
		const std::string						_username;
		const std::string						_realname;
		bool									_connected;
		Logger									_logger;
		std::map<enum e_commands, AAction *>	_actions;
	
	private:

		ABot(void);

		void									setConnected(bool connected);
		void									setSocket(int socket);

		void									connect(void);
		void									authenticate(void);
		void									routine(void);
		void									disconnect(void);
		void									sendMessage(const AMessage &msg) const; //sends a message to the server
		const AMessage							*receiveMessage(void) const; //receives a message from the server

		int										_server_socket; // socket to the 
		std::string								_server_ip;
		uint16_t								_server_port;
		std::string								_server_password;
};

/*

Action:
 -trigger command
 -beExecuted: funzione da chiamare (restituira' un CommandMessage finale da inviare al server)



*/

#endif