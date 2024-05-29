/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/29 13:41:14 by craimond         ###   ########.fr       */
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

class Client;
class Client;
class Logger;

class Channel
{
	public:
		explicit Channel(Logger &logger, const std::string &name, Client &op, const std::string &key = "");
		Channel(const Channel &copy);
		~Channel(void);
	
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
		void									removeMember(Client &user);
		const std::set<Client *>				&getOperators(void) const;
		void									setOperators(const std::set<Client *> &new_operators);
		void									addOperator(const std::string &nickname);
		void									removeOperator(const std::string &nickname);
		const std::set<Client *>				&getPendingInvitations(void) const;
		void									setPendingInvitations(const std::set<Client *> &new_pending_invitations);
		void									addPendingInvitation(Client &user);
		void									removePendingInvitation(Client &user);
		const std::vector<bool>					&getModes(void) const;
		void									setModes(const std::vector<bool> &modes, const std::vector<std::string> &params);
		bool									getMode(const char mode) const;
		void									setMode(const char mode, const bool status, const std::string &param = "");

		void									receiveMessage(const struct s_commandMessage &message) const;
		bool									isOperator(const Client &user) const;
		bool									isMember(const std::string &nickname) const;
		bool									isMember(const Client &user) const;
		std::string								getMembersString(void) const;

	private:

		std::string								_name;
		std::string								_key;
		std::string								_topic;
		uint32_t								_member_limit;
		std::map<std::string, Client *>			_members; // {nickname, client}
		std::set<Client *>						_operators;
		std::set<Client *>						_pending_invitations;
		std::vector<bool>						_modes;
		Logger									&_logger;

		void									checkName(const std::string &name) const;
		void									checkKey(const std::string &key) const;
};

#endif