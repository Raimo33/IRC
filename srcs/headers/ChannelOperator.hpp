/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:25:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include <string>

# include "irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

using namespace std;
using namespace irc;

//L'operator e' un Client che assume ruolo di operator solo in determinati canali
//l'operator puo' essere un operator in piu' canali contemporaneamente
class ChannelOperator : public Client
{
	public:
		explicit ChannelOperator(const Client &user); //per promuovere l'Client a operator
		ChannelOperator(const ChannelOperator &copy);
		~ChannelOperator(void);

		void	channelKick(const Client &user, Channel &channel) const; //chiama removeMember di Channel
		void	channelInvite(Client &user, Channel &channel) const; //chiama addInvite di Channel
		void	channelTopicSet(Channel &channel, const string &new_topic) const; // chiama setTopic di Channel
		void	channelModeChange(Channel &channel, const t_channel_modes &mode, const bool status) const; // chiama setMode di Channel
	
	private:
		void	checkPrivilege(const Channel &channel) const;
};

#endif