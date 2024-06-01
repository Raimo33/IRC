/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 16:31:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
# define ABOT_HPP

//TODO ogni volta che il bot riceve un messaggio indietro dal server deve interpetarlo e farsi il suo database di roba di riferimento
//tipo: appena entra in un canale crea l'oggetto Channel, appena riceve la Namreply si fa la lista degli utenti e se li salva nell'oggetto Channel

#include <string>
#include <map>

#include "Logger.hpp"

class Server;
class Action;
class Channel;
class Client;
class Logger;

class ABot
{
	public:
		explicit ABot(const std::string &nickname, const std::string &username, const std::string &realname);
		ABot(const ABot &copy);
		virtual ~ABot(void) = 0;

		const std::string				&getNickname(void) const;
		const std::string				&getUsername(void) const;
		const std::string				&getRealname(void) const;
		bool							isConnected(void) const;
		int								getSocket(void) const;
		void							setLogger(const Logger &logger);
		const Logger					&getLogger(void) const;

		virtual void					run(void) = 0;
	
	protected:
		void							setConnected(bool connected);
		void							setSocket(int socket);

		void							joinChannel(Channel &channel, const std::string &key = "");
		void							leaveChannel(Channel &channel, const std::string &reason = "");
		void							sendMessage(const Channel &channel, const std::string &text); //:nickname PRIVMSG #channel|nickname :text
		void							sendMessage(const Client &user, const std::string &text); //:nickname PRIVMSG #channel|nickname :text
		void							connect(const std::string &ip, const std::string &port);
		void							disconnect(void);
		//receive message (Message o text)				

		const std::string				_nickname;
		const std::string				_username;
		const std::string				_realname;
		bool							_connected;
		Logger							_logger;
	
	private:
		ABot(void);

		void							_authenticate(void); //invia la sequenza di comandi per autenticarsi

		int								_socket;
		std::map<std::string, Action>	_actions;
		std::map<std::string, Channel>	_channels;
};

#endif