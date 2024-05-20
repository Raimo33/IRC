/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/20 14:45:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define N_MODES 5
# define MAX_CHANNEL_NAME_LEN 200
# define MAX_TOPIC_LEN 390
# define MAX_KEY_LEN 50
# define DEFAULT_MEMBER_LIMIT 50

# include <map>
# include <string>
# include <vector>
# include <algorithm>
# include <stdint.h>
# include <stdexcept>

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
class Client;
class Message;
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
		const Client							&getOperator(const string &username) const;
		Client									&addOperator(ChannelOperator *op);
		void									removeOperator(const ChannelOperator &op);
		const map<string, Client *>				&getMembers(void) const;
		void									setMembers(const map<string, Client *> &new_members);
		const Client							&getMember(const string &username) const;
		void									addMember(Client &user);
		void									removeMember(const Client &user);
		const map<string, Client *>				&getPendingInvitations(void) const;
		void									setPendingInvitations(const map<string, Client *> &new_invitations);
		const Client							&getPendingInvitation(const string &username) const;
		void									addPendingInvitation(Client *user);
		void									removePendingInvitation(const Client &user);
		const bool								*getModes(void) const;
		void									setModes(const bool new_modes[N_MODES]);
		bool									getMode(const t_channel_modes &mode) const;
		void									setMode(const t_channel_modes &mode, const bool value);

		void									receiveMessage(const Message &msg) const;
		
		class       							InvalidNameException; //constructor
		class									InvalidTopicException; //setTopic
		class									InvalidKeyException; //setKey, Client::joinChannel
		class									ChannelFullException; //addMember
		class									UserAlreadyMemberException; //addMember
		class									UserAlreadyOperatorException; //promoteOperator
		class									UserNotOperatorException; //removeOperator, demoteOperator
		class									UserNotMemberException; //removeMember
		class									UnknownModeException; //setMode
		
		friend class 							ChannelOperator;

	private:
		void									promoteOperator(const string &username);
		void									demoteOperator(const string &username);

		string									_name; //deve iniziare con # o & e contenere massimo 200 caratteri, caratteri vietati: (spazio, ^G, virgola)
		string									_key; //la chiave del canale non viene hashata, si conserva quella raw
		string									_topic;
		uint32_t								_member_limit;
		map<string, ChannelOperator *>			_operators; // {username, operator}
		map<string, Client *>					_members; // {username, user}
		map<string, Client *>					_pending_invitations; // {username, user} (il channel operator puo fare /invite)
		bool									_modes[N_MODES];
};

class Channel::InvalidNameException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::InvalidTopicException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::InvalidKeyException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::ChannelFullException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::UserAlreadyMemberException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::UserAlreadyOperatorException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::UserNotOperatorException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::UserNotMemberException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

class Channel::UnknownModeException: public exception
{
	public:
		virtual const char *what(void) const throw();
};

#endif