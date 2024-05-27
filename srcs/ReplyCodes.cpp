/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyCodes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 02:55:58 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 12:00:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ReplyCodes.hpp"

#include <map>
#include <string>

using std::string;
using std::map;
using std::make_pair;

namespace irc
{
	const map<uint16_t, string> create_reply_codes(void)
	{
		map<uint16_t, string> reply_codes;
		reply_codes.insert(make_pair(RPL_WELCOME, "Welcome to the Internet Relay Network"));
		reply_codes.insert(make_pair(RPL_YOURHOST, "Your host is running version"));
		reply_codes.insert(make_pair(RPL_CREATED, "This server was created"));
		reply_codes.insert(make_pair(RPL_MYINFO, "My Info"));
		reply_codes.insert(make_pair(RPL_BOUNCE, "Try server, port number"));
		reply_codes.insert(make_pair(RPL_USERHOST, "User host"));
		reply_codes.insert(make_pair(RPL_ISON, "Is on"));
		reply_codes.insert(make_pair(RPL_AWAY, "Away"));
		reply_codes.insert(make_pair(RPL_UNAWAY, "You are no longer marked as being away"));
		reply_codes.insert(make_pair(RPL_NOWAWAY, "You have been marked as being away"));
		reply_codes.insert(make_pair(RPL_WHOISUSER, "Whois user"));
		reply_codes.insert(make_pair(RPL_WHOISSERVER, "Whois server"));
		reply_codes.insert(make_pair(RPL_WHOISOPERATOR, "Whois operator"));
		reply_codes.insert(make_pair(RPL_WHOISIDLE, "Whois idle"));
		reply_codes.insert(make_pair(RPL_ENDOFWHOIS, "End of WHOIS list"));
		reply_codes.insert(make_pair(RPL_WHOISCHANNELS, "Whois channels"));
		reply_codes.insert(make_pair(RPL_WHOWASUSER, "Who was user"));
		reply_codes.insert(make_pair(RPL_ENDOFWHOWAS, "End of WHOWAS"));
		reply_codes.insert(make_pair(RPL_LISTSTART, "Channel :Users Name"));
		reply_codes.insert(make_pair(RPL_LIST, "List"));
		reply_codes.insert(make_pair(RPL_LISTEND, "End of LIST"));
		reply_codes.insert(make_pair(RPL_CHANNELMODEIS, "Channel mode is"));
		reply_codes.insert(make_pair(RPL_NOTOPIC, "No topic is set"));
		reply_codes.insert(make_pair(RPL_TOPIC, "Topic"));
		reply_codes.insert(make_pair(RPL_INVITING, "Inviting"));
		reply_codes.insert(make_pair(RPL_SUMMONING, "Summoning"));
		reply_codes.insert(make_pair(RPL_VERSION, "Version"));
		reply_codes.insert(make_pair(RPL_WHOREPLY, "Who reply"));
		reply_codes.insert(make_pair(RPL_ENDOFWHO, "End of WHO list"));
		reply_codes.insert(make_pair(RPL_NAMREPLY, "Name reply"));
		reply_codes.insert(make_pair(RPL_ENDOFNAMES, "End of NAMES list"));
		reply_codes.insert(make_pair(RPL_LINKS, "Links"));
		reply_codes.insert(make_pair(RPL_ENDOFLINKS, "End of LINKS list"));
		reply_codes.insert(make_pair(RPL_BANLIST, "Ban list"));
		reply_codes.insert(make_pair(RPL_ENDOFBANLIST, "End of BAN list"));
		reply_codes.insert(make_pair(RPL_INFO, "Info"));
		reply_codes.insert(make_pair(RPL_ENDOFINFO, "End of INFO list"));
		reply_codes.insert(make_pair(RPL_MOTDSTART, "Message of the day - "));
		reply_codes.insert(make_pair(RPL_MOTD, "Motd"));
		reply_codes.insert(make_pair(RPL_ENDOFMOTD, "End of MOTD command"));
		reply_codes.insert(make_pair(RPL_YOUREOPER, "You are now an IRC operator"));
		reply_codes.insert(make_pair(RPL_REHASHING, "Rehashing"));
		reply_codes.insert(make_pair(RPL_TIME, "Time"));
		reply_codes.insert(make_pair(RPL_USERSSTART, "Users start"));
		reply_codes.insert(make_pair(RPL_USERS, "Users"));
		reply_codes.insert(make_pair(RPL_ENDOFUSERS, "End of users"));
		reply_codes.insert(make_pair(RPL_NOUSERS, "Nobody logged in"));
		reply_codes.insert(make_pair(ERR_NOSUCHNICK, "No such nick/channel"));
		reply_codes.insert(make_pair(ERR_NOSUCHSERVER, "No such server"));
		reply_codes.insert(make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
		reply_codes.insert(make_pair(ERR_CANNOTSENDTOCHAN, "Cannot send to channel"));
		reply_codes.insert(make_pair(ERR_TOOMANYCHANNELS, "You have joined too many channels"));
		reply_codes.insert(make_pair(ERR_WASNOSUCHNICK, "There was no such nickname"));
		reply_codes.insert(make_pair(ERR_TOOMANYTARGETS, "Duplicate recipients. No message delivered"));
		reply_codes.insert(make_pair(ERR_NOORIGIN, "No origin specified"));
		reply_codes.insert(make_pair(ERR_NORECIPIENT, "No recipient given"));
		reply_codes.insert(make_pair(ERR_NOTEXTTOSEND, "No text to send"));
		reply_codes.insert(make_pair(ERR_NOTOPLEVEL, "No toplevel domain specified"));
		reply_codes.insert(make_pair(ERR_WILDTOPLEVEL, "Wildcard in toplevel domain"));
		reply_codes.insert(make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
		reply_codes.insert(make_pair(ERR_NOMOTD, "MOTD File is missing"));
		reply_codes.insert(make_pair(ERR_NOADMININFO, "No administrative info available"));
		reply_codes.insert(make_pair(ERR_FILEERROR, "File error"));
		reply_codes.insert(make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
		reply_codes.insert(make_pair(ERR_ERRONEOUSNICKNAME, "Erroneous nickname"));
		reply_codes.insert(make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
		reply_codes.insert(make_pair(ERR_NICKCOLLISION, "Nickname collision"));
		reply_codes.insert(make_pair(ERR_USERNOTINCHANNEL, "They aren't on that channel"));
		reply_codes.insert(make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
		reply_codes.insert(make_pair(ERR_USERONCHANNEL, "is already on channel"));
		reply_codes.insert(make_pair(ERR_NOLOGIN, "User not logged in"));
		reply_codes.insert(make_pair(ERR_SUMMONDISABLED, "SUMMON has been disabled"));
		reply_codes.insert(make_pair(ERR_USERSDISABLED, "USERS has been disabled"));
		reply_codes.insert(make_pair(ERR_NOTREGISTERED, "You have not registered"));
		reply_codes.insert(make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
		reply_codes.insert(make_pair(ERR_ALREADYREGISTRED, "You may not reregister"));
		reply_codes.insert(make_pair(ERR_NOPERMFORHOST, "Your host isn't among the privileged"));
		reply_codes.insert(make_pair(ERR_PASSWDMISMATCH, "Password incorrect"));
		reply_codes.insert(make_pair(ERR_YOUREBANNEDCREEP, "You are banned from this server"));
		reply_codes.insert(make_pair(ERR_KEYSET, "Channel key already set"));
		reply_codes.insert(make_pair(ERR_CHANNELISFULL, "Cannot join channel (+l)"));
		reply_codes.insert(make_pair(ERR_UNKNOWNMODE, "is unknown mode char to me"));
		reply_codes.insert(make_pair(ERR_INVITEONLYCHAN, "Cannot join channel (+i)"));
		reply_codes.insert(make_pair(ERR_BANNEDFROMCHAN, "Cannot join channel (+b)"));
		reply_codes.insert(make_pair(ERR_BADCHANNELKEY, "Cannot join channel (+k)"));
		reply_codes.insert(make_pair(ERR_NOPRIVILEGES, "Permission Denied- You're not an IRC operator"));
		reply_codes.insert(make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
		reply_codes.insert(make_pair(ERR_CANTKILLSERVER, "You can't kill a server!"));
		reply_codes.insert(make_pair(ERR_NOOPERHOST, "No O-lines for your host"));
		reply_codes.insert(make_pair(ERR_UMODEUNKNOWNFLAG, "Unknown MODE flag"));
		reply_codes.insert(make_pair(ERR_USERSDONTMATCH, "Cannot change mode for other users"));
		return reply_codes;
	}

    const map<uint16_t, string> reply_codes = create_reply_codes();
}
