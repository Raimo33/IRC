/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 11:59:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define N_MODES 5

# include <unordered_map>
# include <string>
# include <vector>
# include <algorithm>
# include "ChannelOperator.hpp"

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
		Channel();
		Channel(const Channel &copy);
		~Channel();
		Channel		&operator=(const Channel &rhs);
		string		getName() const;
		string		getTopic() const;
		bool		getMode(const t_channel_modes &mode) const;
		string		getOperator(const string &nickname) const;
		string		getMember(const string &nickname) const;
		string		getRequest(const string &nickname) const;
		uint32_t	getMembersCount() const;
		void		addRequest(const User &user);
		class 		UserNotInChannelException;
		class		NotOperatorException;
		class		NotExistingModeException;
		class		InvalidCredentialsException;
	private:
		//solo l'operator puo' cambiare modes e topic del canale (operator sara' un friend di Channel)
		friend class ChannelOperator;
		void	setName(const string &new_name);
		void	setTopic(const string &new_topic);
		void	setMode(const ChannelOperator &op, const t_channel_modes &mode, const bool status);
		void	addUser(const User &user);
		void	addOperator(const ChannelOperator &op);
		void	removeUser(const User &user);
		string	_name; //deve iniziare con # o & e contenere massimo 200 caratteri, caratteri vietati: (spazio, ^G, virgola)
		string	_topic;
		unordered_map<string, const ChannelOperator *>	_operators; // {nickname, operator}
		unordered_map<string, const User *>	_members; // {nickname, user}
		unordered_map<string, const User *>	_requests; // {nickname, user}
		bool	_modes[N_MODES];
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

#endif