/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABot.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:45:18 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 00:17:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABOT_HPP
# define ABOT_HPP

#include <string>
#include <map>

class Server;
class Action;
class Channel;
class Client;

class ABot
{
	public:
		explicit ABot(const std::string &nickname, const std::string &username, const std::string &realname);
		ABot(const ABot &copy);
		virtual ~ABot(void) = 0;

		ABot							&operator=(const ABot &copy);

		const std::string				&getNickname(void) const;
		const std::string				&getUsername(void) const;
		const std::string				&getRealname(void) const;
		int								getSocket(void) const;
		void							setSocket(int socket);
		bool							isConnected(void) const;
		void							setConnected(bool connected);
		Server							&getServer(void) const;
		void							setServer(Server &server);

		void							connect(const Server &server);
		void							disconnect(void);
		virtual void					run(void) = 0;
	
	protected:
		void							sendMessage(const Channel &channel, const std::string &text); //:nickname PRIVMSG #channel|nickname :message
		void							sendMessage(const Client &user, const std::string &text); //:nickname PRIVMSG #channel|nickname :message

		const std::string				_nickname;
		const std::string				_username;
		const std::string				_realname;
		int								_socket;
		bool							_connected;
		Server							*_server;
	
	private:
		ABot(void);
		
		void							_authenticate(void);

};

#endif