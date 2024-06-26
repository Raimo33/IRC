/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 18:14:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>
#include <poll.h>
#include <stdint.h>
#include <string>
#include <vector>

class Server;
class Channel;
class Logger;
class AMessage;

class Client
{
  public:
	explicit Client(Logger &logger, Server *server, const int socket, const std::string &ip_addr, const uint16_t port);
	Client(const Client &copy);
	~Client(void);

	const std::map<std::string, Channel *> &getChannels(void) const;
	void									setChannels(const std::map<std::string, Channel *> &channels);
	Channel								   &getChannel(const std::string &channel_name) const;
	void									addChannel(Channel &channel);
	void									removeChannel(Channel &channel);
	const std::string					   &getNickname(void) const;
	void									setNickname(const std::string &nickname);
	const std::string					   &getUsername(void) const;
	void									setUsername(const std::string &username);
	const std::string					   &getRealname(void) const;
	void									setRealname(const std::string &realname);
	bool									getIsConnected(void) const;
	void									setIsConnected(bool is_connected);
	bool									getIsAuthenticated(void) const;
	void									setAuthenticated(bool is_authenticated);
	uint16_t								getPort(void) const;
	const std::string					   &getIpAddr(void) const;
	int										getSocket(void) const;
	std::string								getPk(void) const;
	const pollfd						   &getPollfd(void) const;
	Server								   &getServer(void) const;

	void joinChannel(Channel &channel, const std::string &key = "");
	void leaveChannel(Channel &channel, const std::string &reason = "");
	void sendMessage(const Channel &channel, const AMessage &msg) const;
	void sendMessage(const Client &receiver, const AMessage &msg) const;
	void receiveMessage(const AMessage *msg) const;

	void kick(Client &user, Channel &channel, const std::string &reason = "") const;
	void invite(Client &user, Channel &channel) const;
	void topicSet(Channel &channel, const std::string &new_topic) const;
	void modeChange(Channel &channel, const char mode, const bool status, const std::string &param = "") const;
	void modesChange(Channel &channel, const std::map<char, bool> &modes, const std::vector<std::string> &params) const;
	void promoteOperator(Channel &channel, Client &user) const;
	void demoteOperator(Channel &channel, Client &op) const;

  private:
	void checkPrivilege(const Channel &channel) const;

	std::map<std::string, Channel *> _channels;
	std::string						 _nickname;
	std::string						 _username;
	std::string						 _realname;
	bool							 _is_connected;
	bool							 _is_authenticated;

	const uint16_t	  _port;
	const std::string _ip_addr;
	const int		  _socket;
	const std::string _pk;
	Server			 *_server;
	Logger			 &_logger;
};

#endif
