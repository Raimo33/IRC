/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyCodes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 02:55:58 by craimond          #+#    #+#             */
/*   Updated: 2024/05/29 13:48:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ReplyCodes.hpp"

#include <map>
#include <string>

using std::string;
using std::map;
using std::make_pair;

const map<enum e_replyCodes, string> create_reply_codes(void)
{
	map<enum e_replyCodes, string> reply_codes;
	reply_codes.insert(make_pair(RPL_WELCOME, "Welcome to the Internet Relay Network"));
	reply_codes.insert(make_pair(RPL_YOURHOST, "Your host is running version"));
	reply_codes.insert(make_pair(RPL_CHANNELMODEIS, "Channel mode is"));
	reply_codes.insert(make_pair(RPL_NOTOPIC, "No topic is set"));
	reply_codes.insert(make_pair(RPL_TOPIC, "Topic"));
	reply_codes.insert(make_pair(RPL_INVITING, "Inviting"));
	reply_codes.insert(make_pair(RPL_NAMREPLY, "Name reply"));
	reply_codes.insert(make_pair(RPL_ENDOFNAMES, "End of NAMES list"));
	reply_codes.insert(make_pair(RPL_YOUREOPER, "You are now an IRC operator"));
	reply_codes.insert(make_pair(RPL_NOTOPERANYMORE, "You are no longer an IRC operator"));
	reply_codes.insert(make_pair(ERR_NOSUCHNICK, "No such nick/channel"));
	reply_codes.insert(make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	reply_codes.insert(make_pair(ERR_TOOMANYCHANNELS, "You have joined too many channels"));
	reply_codes.insert(make_pair(ERR_NORECIPIENT, "No recipient given"));
	reply_codes.insert(make_pair(ERR_NOTEXTTOSEND, "No text to send"));
	reply_codes.insert(make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
	reply_codes.insert(make_pair(ERR_FILEERROR, "File error"));
	reply_codes.insert(make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
	reply_codes.insert(make_pair(ERR_ERRONEOUSNICKNAME, "Erroneous nickname"));
	reply_codes.insert(make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
	reply_codes.insert(make_pair(ERR_USERNOTINCHANNEL, "They aren't on that channel"));
	reply_codes.insert(make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
	reply_codes.insert(make_pair(ERR_USERONCHANNEL, "is already on channel"));
	reply_codes.insert(make_pair(ERR_NOLOGIN, "User not logged in"));
	reply_codes.insert(make_pair(ERR_NOTREGISTERED, "You have not registered"));
	reply_codes.insert(make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	reply_codes.insert(make_pair(ERR_ALREADYREGISTRED, "You may not reregister"));
	reply_codes.insert(make_pair(ERR_PASSWDMISMATCH, "Password incorrect"));
	reply_codes.insert(make_pair(ERR_CHANNELISFULL, "Cannot join channel (+l)"));
	reply_codes.insert(make_pair(ERR_UNKNOWNMODE, "is unknown mode char to me"));
	reply_codes.insert(make_pair(ERR_INVITEONLYCHAN, "Cannot join channel (+i)"));
	reply_codes.insert(make_pair(ERR_BADCHANNELKEY, "Cannot join channel (+k)"));
	reply_codes.insert(make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
	return reply_codes;
}

const map<enum e_replyCodes, string> reply_codes = create_reply_codes();
