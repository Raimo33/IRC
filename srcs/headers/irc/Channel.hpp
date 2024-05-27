/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 22:07:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>
# include <set>
# include <stdint.h>
# include <stdexcept>

namespace irc
{
	class Client;
	class Client;
	class Logger;

	class Channel //TODO valutare se renderlo una interfaccia e renderlo la base di PublicChannel, PrivateChannel e SecretChannel
	{
		public:
			explicit Channel(Logger &logger, const std::string &name, Client &op); //on creation there must be at least one operator
			Channel(Logger &logger, const std::string &name, const std::string &key, Client &op);
			Channel(const Channel &copy);
			~Channel(void);
		
			//TODO valutare quali metodi rendere private, non possono stare tutti public, e' una bad practice
			const std::string						&getName(void) const;
			void									setName(const std::string &new_name);
			const std::string						&getKey(void) const;
			void									setKey(const std::string &new_key);
			const std::string						&getTopic(void) const;
			void									setTopic(const std::string &new_topic);
			uint32_t								getMemberLimit(void) const;
			void									setMemberLimit(const uint32_t new_limit);
			const std::map<std::string, Client *>	&getMembers(void) const;
			void									setMembers(const std::map<std::string, Client *> &new_members);
			const Client							&getMember(const std::string &nickname) const;
			void									addMember(Client &user);
			void									removeMember(const Client &user);
			const std::set<Client *>				&getOperators(void) const;
			void									setOperators(const std::set<Client *> &new_operators);
			void									addOperator(Client &op);
			void									removeOperator(Client &op);
			const std::set<Client *>				&getPendingInvitations(void) const;
			void									setPendingInvitations(const std::set<Client *> &new_pending_invitations);
			void									addPendingInvitation(Client &user);
			void									removePendingInvitation(Client &user);
			const std::vector<bool>					&getModes(void) const;
			void									setModes(const std::vector<bool> &modes, const std::vector<std::string> &params);
			bool									getMode(const char mode) const;
			void									setMode(const char mode, const bool status, const std::string &param = "");

			void									receiveMessage(const struct s_commandContent &message) const;
			bool									isOperator(const Client &user) const;
			std::string								getMembersString(void) const;

			// friend class 									Client;

		private:

			std::string								_name;
			std::string								_key;
			std::string								_topic;
			uint32_t								_member_limit;
			std::map<std::string, Client *>			_members;
			std::set<Client *>						_operators;
			std::set<Client *>						_pending_invitations;
			std::vector<bool>						_modes;
			Logger									&_logger;

			void									checkName(const std::string &name) const;
			void									checkKey(const std::string &key) const;
	};
}

#endif