/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/15 14:57:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define N_MODES 5

# include <map>
# include <string>
# include <vector>
# include <algorithm>
# include <stdint.h>

using namespace std;

typedef enum e_channel_modes
{
	MODE_I = 0,
	MODE_T = 1,
	MODE_K = 2,
	MODE_O = 3,
	MODE_L = 4
}	t_channel_modes;

class ChannelOperator;
class User;

class Channel
{
	public:
		Channel(void);
		Channel(const Channel &copy);
		~Channel(void);
		Channel		&operator=(const Channel &rhs);
		string		getName(void) const;
		string		getKey(void) const;
		string		getTopic(void) const;
		bool		getMode(const t_channel_modes &mode) const;
		User		&getOperator(const string &nickname) const;
		User		&getMember(const string &nickname) const;
		User		&getRequest(const string &nickname) const;
		uint32_t	getMembersCount(void) const;
		void		addRequest(User &user);
		class 		UserNotInChannelException;
		class		NotOperatorException;
		class		NotExistingModeException;
		class		InvalidCredentialsException;
		class		MemberNotFoundException;
		class		OperatorNotFoundException;
		class		RequestNotFoundException;
	private:
		//solo l'operator puo' cambiare modes e topic del canale (operator sara' un friend di Channel)
		friend class ChannelOperator;
		Channel(const string &name, const string &key, const string &topic);
		void	setName(const string &new_name);
		void	setKey(const string &new_key);
		void	setTopic(const string &new_topic);
		void	setMode(const ChannelOperator &op, const t_channel_modes &mode, const bool status);
		void	addUser(User &user);
		void	addOperator(ChannelOperator &op);
		void	removeUser(const User &user);
		string							_name; //deve iniziare con # o & e contenere massimo 200 caratteri, caratteri vietati: (spazio, ^G, virgola)
		string							_key;
		string							_topic;
		map<string, ChannelOperator *>	_operators; // {nickname, operator}
		map<string, User *>				_members; // {nickname, user}
		map<string, User *>				_requests; // {nickname, user}
		bool							_modes[N_MODES];
		
		
};

class Channel::UserNotInChannelException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::NotOperatorException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::NotExistingModeException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::InvalidCredentialsException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::MemberNotFoundException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::OperatorNotFoundException : public exception
{
	public:
		virtual const char	*what() const throw();
};

class Channel::RequestNotFoundException : public exception
{
	public:
		virtual const char	*what() const throw();
};

#endif