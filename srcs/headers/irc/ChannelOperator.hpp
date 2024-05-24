/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 17:50:32 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include <string>

# include "Client.hpp"
# include "Channel.hpp"

//L'operator e' un Client che assume ruolo di operator solo in determinati canali
//l'operator puo' essere un operator in piu' canali contemporaneamente

namespace irc
{
	class Client;
	class Channel;

	class ChannelOperator : public Client
	{
		public:
			explicit ChannelOperator(const Client &user); //per promuovere l'Client a operator
			ChannelOperator(const ChannelOperator &copy);
			~ChannelOperator(void);

			void	kick(Client &user, Channel &channel, const std::string &reason = "") const; //chiama removeMember di Channel e removeChannel di Client
			void	invite(Client &user, Channel &channel) const; //chiama addInvite di Channel
			void	topicSet(Channel &channel, const std::string &new_topic) const; // chiama setTopic di Channel
			void	modeChange(Channel &channel, const t_channel_modes &mode, const bool status) const; // chiama setMode di Channel
			void	promoteOperator(Channel &channel, const Client &user) const; // chiama addOperator di Channel //TODO d
			void	demoteOperator(Channel &channel, ChannelOperator &op) const; // chiama removeOperator di Channel //TODO d

		private:
			void	checkPrivilege(const Channel &channel) const;
	};
}

#endif