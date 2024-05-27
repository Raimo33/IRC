/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOperator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 19:15:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO togliere la distizione con ChannelOperator

// #ifndef OPERATOR_HPP
// # define OPERATOR_HPP

// # include <string>
// # include <vector>

// # include "Client.hpp"
// # include "Channel.hpp"

// //L'operator e' un Client che assume ruolo di operator solo in determinati canali
// //l'operator puo' essere un operator in piu' canali contemporaneamente

// namespace irc
// {
// 	class Client;
// 	class Channel;

// 	class ChannelOperator : public Client
// 	{
// 		public:
// 			explicit ChannelOperator(const Client &user); //per promuovere l'Client a operator
// 			ChannelOperator(const ChannelOperator &copy);
// 			~ChannelOperator(void);

// 			void	kick(Client &user, Channel &channel, const std::string &reason = "") const; //chiama removeMember di Channel e removeChannel di Client
// 			void	invite(Client &user, Channel &channel) const; //chiama addInvite di Channel
// 			void	topicSet(Channel &channel, const std::string &new_topic) const; // chiama setTopic di Channel
// 			void	modeChange(Channel &channel, const char mode, const bool status, const std::string &param = "") const; // chiama setMode di Channel
// 			void	modesChange(Channel &channel, const std::vector<bool> &modes, const std::vector<std::string> &params) const; // chiama setModes di Channel
// 			void	promoteOperator(Channel &channel, const Client &user) const; // chiama addOperator di Channel
// 			void	demoteOperator(Channel &channel, ChannelOperator &op) const; // chiama removeOperator di Channel

// 		private:
// 			void	checkPrivilege(const Channel &channel) const;
// 	};
// }

#endif