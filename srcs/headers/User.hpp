/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 13:52:54 by craimond         ###   ########.fr       */
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

		string	getNickname(void) const;
		string	getUsername(void) const;
		bool	getIsAuthenticated(void) const;
		void	setNickname(const string &nickname);
		void	setUsername(const string &username);
		void	setAuthenticated(bool is_authenticated);
		void	setPwdHash(const string &password);
		void	joinChannel(Channel &channel);
		void 	joinChannel(Channel &channel, const string &key);

		class	TooManyChannelsException; //joinChannel
		class	AlreadyAuthenticatedException; //setAuthenticated
		class	InvalidCredentialsException; //setAuthenticated
		class	NotAuthenticatedException; //joinChannel

	protected:

		map<string, const Channel *>	_channels; // {channel_name, channel}
		string							_nickname;
		string							_username;
		size_t							_pwd_hash;
		bool							_is_authenticated; //true se l'utente ha effettuato il login, serve per fargli fare la registrazione la prima volta
};

class User::TooManyChannelsException: public exception
{
	public:
		virtual const char	*what() const throw();
};

class User::AlreadyAuthenticatedException: public exception
{
	public:
		virtual const char	*what() const throw();
};

class User::InvalidCredentialsException: public exception
{
	public:
		virtual const char	*what() const throw();
};

class User::NotAuthenticatedException: public exception
{
	public:
		virtual const char	*what() const throw();
};

#endif