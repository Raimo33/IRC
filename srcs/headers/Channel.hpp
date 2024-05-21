/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:25:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>

# include "irc.hpp"
# include <string>
# include <vector>
# include <algorithm>
# include <stdint.h>
# include <stdexcept>

# include "Standards.hpp"

using namespace std;
using namespace irc;

typedef enum e_channel_modes
{
	MODE_I = 0,
	MODE_T = 1,
	MODE_K = 2,
	MODE_O = 3,
	MODE_L = 4
}	t_channel_modes;

class Channel
{
	public:
		explicit Channel(const string &name, ChannelOperator &op); //on creation there must be at least one operator
		Channel(const string &name, const string &key, ChannelOperator &op);
		Channel(const Channel &copy);
		~Channel(void);
	
		const string							&getName(void) const;
		void									setName(const string &new_name);
		const string							&getKey(void) const;
		void									setKey(const string &new_key);
		const string							&getTopic(void) const;
		void									setTopic(const string &new_topic);
		uint32_t								getMemberLimit(void) const;
		void									setMemberLimit(const uint32_t new_limit);
		const map<string, ChannelOperator *>	&getOperators(void) const;
		void									setOperators(const map<string, ChannelOperator *> &new_operators);
		const Client							&getOperator(const string &nickname) const;
		Client									&addOperator(ChannelOperator *op);
		void									removeOperator(const ChannelOperator &op);
		const map<string, Client *>				&getMembers(void) const;
		void									setMembers(const map<string, Client *> &new_members);
		const Client							&getMember(const string &nickname) const;
		void									addMember(Client &user);
		void									removeMember(const Client &user);
		const map<string, Client *>				&getPendingInvitations(void) const;
		void									setPendingInvitations(const map<string, Client *> &new_invitations);
		const Client							&getPendingInvitation(const string &nickname) const;
		void									addPendingInvitation(Client *user);
		void									removePendingInvitation(const Client &user);
		const bool								*getModes(void) const;
		void									setModes(const bool new_modes[N_MODES]);
		bool									getMode(const t_channel_modes &mode) const;
		void									setMode(const t_channel_modes &mode, const bool value);

		void									receiveMessage(const Message &msg) const;
		
		friend class 							ChannelOperator;

	private:
		void									promoteOperator(const string &nickname);
		void									demoteOperator(const string &nickname);

		string									_name; //deve iniziare con # o & e contenere massimo 200 caratteri, caratteri vietati: (spazio, ^G, virgola)
		string									_key; //la chiave del canale non viene hashata, si conserva quella raw
		string									_topic;
		uint32_t								_member_limit;
		map<string, ChannelOperator *>			_operators; // {nickname, operator}
		map<string, Client *>					_members; // {nickname, user}
		map<string, Client *>					_pending_invitations; // {nickname, user} (il channel operator puo fare /invite)
		bool									_modes[N_MODES];
};

#endif