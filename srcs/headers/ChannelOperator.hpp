/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 11:47:43 by craimond         ###   ########.fr       */
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
class ChannelOperator : public User
{
	public:
		ChannelOperator();
		ChannelOperator(const ChannelOperator &copy);
		~ChannelOperator();
		ChannelOperator &operator=(const ChannelOperator &rhs);

		//valutare se omettere il parametro channel essendo che l'operatore è già in un canale
		void	channelKick(const user &user, const Channel &channel) const;
		void	channelInvite(const user &user, const Channel &channel) const;
		string	channelTopicGet(const Channel &channel) const;
		void	channelTopicSet(const Channel &channel, const string &new_topic) const;
		//seleziona un canale, seleziona una modalita', e sceglie se deve essere attiva o meno (true/false)
		void	channelModeChange(const Channel &channel, const channel_modes &mode, const bool status) const; //questa chiamera' il metodo setMode di Channel
};

#endif