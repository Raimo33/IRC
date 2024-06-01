/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 10:41:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <map>
# include <string>
# include <stdint.h>
# include <poll.h>

class Server;
class Channel;
class Logger;
class Message;

class Client
{
	public:
		explicit Client(Logger &logger, Server *server, const int socket, const std::string &ip_addr, const uint16_t port);
		Client(const Client &copy);
		virtual ~Client(void);

		const std::map<std::string, Channel *>			&getChannels(void) const;
		void											setChannels(const std::map<std::string, Channel *> &channels);
		Channel											&getChannel(const std::string &channel_name) const;
		void											addChannel(Channel &channel);
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
		std::string										getPk(void) const;
		const pollfd									&getPollfd(void) const;
		Server											&getServer(void) const;

		void											joinChannel(Channel &channel, const std::string &key = "");
		void											leaveChannel(Channel &channel, const std::string &reason = "");
		void											sendMessage(const Channel &channel, const Message &msg) const;
		void											sendMessage(const Client &receiver, const Message &msg) const;
		void											receiveMessage(const Message &msg) const;

		void											kick(Client &user, Channel &channel, const std::string &reason = "") const; //chiama removeMember di Channel e removeChannel di Client
		void											invite(Client &user, Channel &channel) const; //chiama addInvite di Channel
		void											topicSet(Channel &channel, const std::string &new_topic) const; // chiama setTopic di Channel
		void											modeChange(Channel &channel, const char mode, const bool status, const std::string &param = "") const; // chiama setMode di Channel
		void											modesChange(Channel &channel, const std::map<char, bool> &modes, const std::vector<std::string> &params) const; // chiama setModes di Channel
		void											promoteOperator(Channel &channel, Client &user); // chiama addOperator di Channel
		void											demoteOperator(Channel &channel, Client &op); // chiama removeOperator di Channel
		
	protected:
	
		void											checkPrivilege(const Channel &channel) const;

		std::map<std::string, Channel *>				_channels; // {channel_name, channel}
		std::string										_nickname;
		std::string										_username;
		std::string										_realname;  //usato solo da WHOIS e company
		bool											_is_connected; // dopo che ha fatto il comando PASS
		bool											_is_authenticated; //dopo che ha fatto il comando NICK e USER

		const uint16_t									_port;
		const std::string								_ip_addr; //aka hostname
		const int										_socket;
		const std::string								_pk; //ip_addr + port
 		Server											*_server;
		Logger											&_logger;
};

#endif
