/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 17:23:25 by craimond         ###   ########.fr       */
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
		User	&operator=(const User &rhs);

		void	setNickname(const string &nickname);
		void	setUsername(const string &username);
		void	joinChannel(Channel &channel);
		void 	joinChannel(Channel &channel, const string &key);
		string	getNickname(void) const;
		string	getUsername(void) const;
		bool	isAuthenticated(void) const;
		class	TooManyChannelsException;
		class	AlreadyAuthenticatedException;
		class	InvalidCredentialsException;
		class	NotAuthenticatedException;
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