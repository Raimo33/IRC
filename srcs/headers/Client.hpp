/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 12:42:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <map>
# include <string>
# include "Channel.hpp"

# define MAX_CHANNELS_PER_USER 10

using namespace std;

class Message;
class PrivateMessage;
class Client
{
	public:
		explicit Client(Server *server, const int socket, const string &ip_addr, const uint16_t port);
		Client(const Client &copy);
		~Client(void);

		const map<string, const Channel *>	&getChannels(void) const;
		void								setChannels(const map<string, const Channel *> &channels);
		const Channel						*getChannel(const string &channel_name) const;
		void								addChannel(const Channel &channel);
		void								removeChannel(const Channel &channel);
		const string						&getNickname(void) const;
		void								setNickname(const string &nickname);
		const string						&getUsername(void) const;
		void								setUsername(const string &username);
		bool								getIsConnected(void) const;
		void								setIsConnected(bool is_connected);
		bool								getIsAuthenticated(void) const;
		void								setAuthenticated(bool is_authenticated);
		uint16_t							getPort(void) const;
		const string						&getIpAddr(void) const;
		int									getSocket(void) const;
		Server								*getServer(void) const;

		void								joinChannel(Channel &channel);
		void								joinChannel(Channel &channel, const string &key);
		void								leaveChannel(Channel &channel);
		void								sendMessage(const Channel &channel, const Message &message) const; //chiama channel.receiveMessage
		void								sendMessage(const User &receiver, const PrivateMessage &message) const;

		class								TooManyChannelsException; //joinChannel
		class								AlreadyConnectedException; //setIsConnected
		class								AlreadyDisconnectedException; //setIsConnected
		class								AlreadyAuthenticatedException; //setAuthenticated
		class								AlreadyDisauthenticatedException; //setAuthenticated
		class								NicknameAlreadyInUseException; //setNickname
		class								NotAuthenticatedException; //joinChannel
		class								UserNotInChannelException; //getChannel, Client::sendMessage

	private:

		map<string, const Channel *>		_channels; // {channel_name, channel}
		string								_nickname;
		string								_username;
		string								_realname;
		bool								_is_connected; // dopo che ha fatto il comando PASS
		bool								_is_authenticated; //dopo che ha fatto il comando NICK e USER

		const uint16_t						_port;
		const string						_ip_addr; //aka hostname
		const int							_socket;
		Server								*_server;
		
};

class Client::TooManyChannelsException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class Client::AlreadyAuthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class Client::NicknameAlreadyInUseException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class Client::NotAuthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class Client::UserNotInChannelException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class Client::AlreadyDisauthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

#endif