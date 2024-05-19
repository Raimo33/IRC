/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:59:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <map>
# include <string>
# include "Channel.hpp"

# define MAX_CHANNELS_PER_USER 10

using namespace std;

class Message;
class PrivateMessage;

class User
{
	public:
		User(void);
		User(const User &copy);
		~User(void);

		const map<string, const Channel *>	&getChannels(void) const;
		void								setChannels(const map<string, const Channel *> &channels);
		const Channel						*getChannel(const string &channel_name) const;
		void								addChannel(const Channel &channel);
		const string						&getNickname(void) const;
		void								setNickname(const string &nickname);
		const string						&getUsername(void) const;
		void								setUsername(const string &username);
		const string						&getPwdHash(void) const;
		void								setPwdHash(const string &pwd_hash);
		bool								getIsAuthenticated(void) const;
		void								setAuthenticated(bool is_authenticated);

		class								TooManyChannelsException; //joinChannel
		class								AlreadyAuthenticatedException; //setAuthenticated
		class								AlreadyDisauthenticatedException; //setAuthenticated
		class								InvalidCredentialsException; //setAuthenticated
		class								NotAuthenticatedException; //joinChannel
		class								UserNotInChannelException; //getChannel

	protected:

		map<string, const Channel *>		_channels; // {channel_name, channel}
		string								_nickname;
		string								_username;
		string								_pwd_hash;
		bool								_is_authenticated; //true se l'utente ha effettuato il login, serve per fargli fare la registrazione la prima volta

	private:
		
};

class User::TooManyChannelsException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class User::AlreadyAuthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class User::InvalidCredentialsException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class User::NotAuthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class User::UserNotInChannelException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

class User::AlreadyDisauthenticatedException: public exception
{
	public:
		virtual const char	*what(void) const throw();
};

#endif