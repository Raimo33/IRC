/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 13:27:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <map>
# include <string>
# include <stdint.h>

namespace irc
{
	class Server;
	class Channel;
	class Message;
	class PrivateMessage;
	class Logger;
	
	class Client
	{
		public:
			explicit Client(Logger &logger, Server *server, const int socket, const std::string &ip_addr, const uint16_t port);
			Client(const Client &copy);
			virtual ~Client(void);

			const std::map<std::string, const Channel *>	&getChannels(void) const;
			void											setChannels(const std::map<std::string, const Channel *> &channels);
			const Channel									&getChannel(const std::string &channel_name) const;
			void											addChannel(const Channel &channel);
			void											removeChannel(const Channel &channel);
			const std::string								&getNickname(void) const;
			void											setNickname(const std::string &nickname);
			const std::string								&getUsername(void) const;
			void											setUsername(const std::string &username);
			const std::string								&getRealname(void) const;
			void											setRealname(const std::string &realname);
			bool											getIsConnected(void) const;
			void											setIsConnected(bool is_connected);
			bool											getIsAuthenticated(void) const;
			void											setAuthenticated(bool is_authenticated);
			uint16_t										getPort(void) const;
			const std::string								&getIpAddr(void) const;
			int												getSocket(void) const;
			Server											&getServer(void) const;

			void											joinChannel(Channel &channel, const std::string &key = "");
			void											leaveChannel(Channel &channel, const std::string &reason = "");
			void											sendMessage(const Channel &channel, const struct s_commandContent &msg) const; //TODO valutare se passare direttamente il messaggio come testo
			void											sendMessage(const Client &receiver, const struct s_commandContent &msg) const;
			void											receiveMessage(const struct s_contentBase &msg) const;
			
		protected:

			std::map<std::string, const Channel *>			_channels; // {channel_name, channel}
			std::string										_nickname;
			std::string										_username;
			std::string										_realname;  //usato solo da WHOIS e company
			bool											_is_connected; // dopo che ha fatto il comando PASS
			bool											_is_authenticated; //dopo che ha fatto il comando NICK e USER

			const uint16_t									_port;
			const std::string								_ip_addr; //aka hostname
			const int										_socket;
			Server											*_server;
			Logger											&_logger;
	};
}

#endif
