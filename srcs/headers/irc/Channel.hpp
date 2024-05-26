/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 17:56:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <string>
# include <vector>
# include <algorithm>
# include <stdint.h>
# include <stdexcept>
# include "Constants.hpp"

namespace irc
{
	class ChannelOperator;
	class Client;

	class Channel //TODO valutare se renderlo una interfaccia e renderlo la base di PublicChannel, PrivateChannel e SecretChannel
	{
		public:
			explicit Channel(const std::string &name, ChannelOperator &op); //on creation there must be at least one operator
			Channel(const std::string &name, const std::string &key, ChannelOperator &op);
			Channel(const Channel &copy);
			~Channel(void);
		
			//TODO valutare quali metodi rendere private, non possono stare tutti public, e' una bad practice
			const std::string						&getName(void) const;
			void									setName(const std::string &new_name);
			const std::string						&getKey(void) const;
			void									setKey(const std::string &new_key);
			const std::string						&getTopic(void) const;
			void									setTopic(const std::string &new_topic, const Client &setter);
			uint32_t								getMemberLimit(void) const;
			void									setMemberLimit(const uint32_t new_limit);
			void									addOperator(ChannelOperator &op);
			void									removeOperator(ChannelOperator &op);
			const std::map<std::string, Client *>	&getMembers(void) const;
			void									setMembers(const std::map<std::string, Client *> &new_members);
			const Client							&getMember(const std::string &nickname) const;
			void									addMember(Client &user);
			void									removeMember(const Client &user);
			const std::map<std::string, Client *>	&getPendingInvitations(void) const;
			void									setPendingInvitations(const std::map<std::string, Client *> &new_invitations);
			const Client							&getPendingInvitation(const std::string &nickname) const;
			void									addPendingInvitation(Client *user);
			void									removePendingInvitation(const Client &user);
			const std::vector<bool>					&getModes(void) const;
			void									setModes(const std::vector<bool> &modes, const std::vector<std::string> &params);
			bool									getMode(const char mode) const;
			void									setMode(const char mode, const bool status, const std::string &param = "");

			void									receiveMessage(const struct s_commandContent &message) const;
			bool									isOperator(const std::string &nickname) const;
			bool									isOperator(const Client *user) const;
			std::string								getMembersString(void) const;

			// friend class 									ChannelOperator;

		private:

			std::string								_name;
			std::string								_key;
			std::string								_topic;
			uint32_t								_member_limit;
			std::map<std::string, Client *>			_members;
			std::map<std::string, Client *>			_pending_invitations;
			std::vector<bool>						_modes;

			void									checkName(const std::string &name) const;
			void									checkKey(const std::string &key) const;
	};
}

#endif