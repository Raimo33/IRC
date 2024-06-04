/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 15:12:21 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

class Client;
class ABot;
class Logger;
class AMessage;

class Channel
{
	public:
	explicit Channel(Logger & logger, const std::string & name, Client & op, const std::string & key = "");
	Channel(const Channel & copy);
	~Channel(void);

	const std::string &						getName(void) const;
	void									setName(const std::string & new_name);
	const std::string &						getKey(void) const;
	void									setKey(const std::string & new_key);
	const std::string &						getTopic(void) const;
	void									setTopic(const std::string & new_topic);
	uint32_t								getMemberLimit(void) const;
	void									setMemberLimit(const uint32_t new_limit);
	const std::map<std::string, Client *> & getMembers(void) const;
	void									setMembers(const std::map<std::string, Client *> & new_members);
	Client &								getMember(const std::string & nickname) const;
	void									addMember(Client & user);
	void									removeMember(const std::string & nickname);
	const std::set<const Client *> &		getOperators(void) const;
	void									setOperators(const std::set<const Client *> & new_operators);
	void									addOperator(const std::string & nickname);
	void									removeOperator(const std::string & nickname);
	const std::set<const Client *> &		getPendingInvitations(void) const;
	void									setPendingInvitations(const std::set<const Client *> & new_pending_invitations);
	void									addPendingInvitation(Client & user);
	void									removePendingInvitation(Client & user);
	const std::map<char, bool> &			getModes(void) const;
	void									setModes(const std::map<char, bool> & modes, const std::vector<std::string> & params, const Client * setter = NULL);
	bool									getMode(const char mode) const;
	void									setMode(const char mode, const bool status, const std::string & param = "", const Client * setter = NULL);

	void		receiveMessage(const AMessage & message, const Client * sender = NULL) const;
	bool		isOperator(const Client & user) const;
	bool		isMember(const std::string & nickname) const;
	bool		isMember(const Client & user) const;
	std::string getMembersString(void) const;

	private:
	const std::map<char, bool> initModes(void) const;
	void					   checkName(const std::string & name) const;
	void					   checkKey(const std::string & key) const;

	std::string						_name;
	std::string						_key;
	std::string						_topic;
	uint32_t						_member_limit;
	std::map<std::string, Client *> _members; // {nickname, client}
	std::set<const Client *>		_operators;
	std::set<const ABot *>			_bots;
	std::set<const Client *>		_pending_invitations;
	std::map<char, bool>			_modes; // {mode, status}
	Logger &						_logger;
};

#endif