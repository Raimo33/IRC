/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 12:50:42 by craimond         ###   ########.fr       */
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
class User;

class Channel
{
	public: 
		explicit Channel(const string &name, ChannelOperator &op); //on creation there must be at least one operator
		Channel(const string &name, const string &key, ChannelOperator &op);
		Channel(const Channel &copy);
		~Channel(void);
	
		string							getName(void) const;
		string							getKey(void) const;
		string							getTopic(void) const;
		bool							getMode(const t_channel_modes &mode) const;
		User							&getOperator(const string &username) const;
		User							&getMember(const string &username) const;
		User							&getRequest(const string &username) const;
		uint32_t						getMembersCount(void) const;
		uint32_t						getMemberLimit(void) const;
		map<string, User *>				getMembers(void) const;
		bool							handleJoinRequest(User &user);
			
		class       					InvalidNameException; //costruttore
		class							InvalidTopicException; //setTopic
		class							InvalidKeyException; //setKey
		class							UserAlreadyMemberException; //addMember
		class							UserAlreadyOperatorException; //promoteOperator
		class							UserNotMemberException; //removeMember
		class							UnknownModeException; //setMode
	private:
		friend class ChannelOperator;
		void							addInvite(User &user);
		void							addMember(User &user);
		void							setKey(const string &new_key);
		void							setTopic(const string &new_topic);
		void							setMode(const t_channel_modes &mode, const bool status);
		void							promoteOperator(const string &username);
		void							removeMember(const User &user);

		const string					_name; //deve iniziare con # o & e contenere massimo 200 caratteri, caratteri vietati: (spazio, ^G, virgola)
		string							_key;
		string							_topic;
		uint32_t						_member_limit;
		map<string, ChannelOperator *>	_operators; // {username, operator}
		map<string, User *>				_members; // {username, user}
		map<string, User *>				_requests; // {username, user}
		map<string, User *>				_pending_invitations; // {username, user} (il channel operator puo fare /invite)
		bool							_modes[N_MODES];
};

#endif