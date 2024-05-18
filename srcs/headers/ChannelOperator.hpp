/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 14:46:38 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

class user;
class Channel;

# include <string>
# include "User.hpp"
# include "Channel.hpp"

using namespace std;

//L'operator e' un user che assume ruolo di operator solo in determinati canali
//l'operator puo' essere un operator in piu' canali contemporaneamente
class ChannelOperator : public User
{
	public:
		explicit ChannelOperator(const User &user); //per promuovere l'user a operator
		ChannelOperator(const ChannelOperator &copy);
		~ChannelOperator(void);

		void	channelKick(const User &user, Channel &channel) const; //chiama removeMember di Channel
		void	channelInvite(User &user, Channel &channel) const; //chiama addInvite di Channel
		string	channelTopicGet(const Channel &channel) const; // chiama getTopic di Channel
		void	channelTopicSet(Channel &channel, const string &new_topic) const; // chiama setTopic di Channel
		void	channelModeChange(Channel &channel, const t_channel_modes &mode, const bool status) const; // chiama setMode di Channel

		class	UserNotInChannelException;
		class	OperatorNotInChannelException;
};

class ChannelOperator::UserNotInChannelException : public exception
{
	public:
		const char	*what(void) const throw();
};

class ChannelOperator::OperatorNotInChannelException : public exception
{
	public:
		const char	*what(void) const throw();
};

#endif