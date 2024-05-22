/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyCodes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 02:55:58 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 03:22:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/ReplyCodes.hpp"
#include <map>
#include <string>

namespace irc
{
	const std::map<uint16_t, std::string> create_reply_codes(void)
	{
		std::map<uint16_t, std::string> reply_codes;
		reply_codes.insert(std::make_pair(RPL_WELCOME, "Welcome"));
		reply_codes.insert(std::make_pair(RPL_YOURHOST, "Your host"));
		reply_codes.insert(std::make_pair(RPL_CREATED, "Created"));
		reply_codes.insert(std::make_pair(RPL_MYINFO, "My info"));
		reply_codes.insert(std::make_pair(RPL_ISUPPORT, "Support"));
		reply_codes.insert(std::make_pair(RPL_BOUNCE, "Bounce"));
		reply_codes.insert(std::make_pair(RPL_MAP, "Map"));
		reply_codes.insert(std::make_pair(RPL_MAPEND, "Map end"));
		reply_codes.insert(std::make_pair(RPL_MAPSTART, "Map start"));
		reply_codes.insert(std::make_pair(RPL_HELLO, "Hello"));
		reply_codes.insert(std::make_pair(RPL_YOURID, "Your ID"));
		reply_codes.insert(std::make_pair(RPL_SAVENICK, "Save nick"));
		reply_codes.insert(std::make_pair(RPL_TRACELINK, "Trace link"));
		reply_codes.insert(std::make_pair(RPL_TRACECONNECTING, "Trace connecting"));
		reply_codes.insert(std::make_pair(RPL_TRACEHANDSHAKE, "Trace handshake"));
		reply_codes.insert(std::make_pair(RPL_TRACEUNKNOWN, "Trace unknown"));
		reply_codes.insert(std::make_pair(RPL_TRACEOPERATOR, "Trace operator"));
		reply_codes.insert(std::make_pair(RPL_TRACEUSER, "Trace user"));
		reply_codes.insert(std::make_pair(RPL_TRACESERVER, "Trace server"));
		reply_codes.insert(std::make_pair(RPL_TRACESERVICE, "Trace service"));
		reply_codes.insert(std::make_pair(RPL_TRACENEWTYPE, "Trace new type"));
		reply_codes.insert(std::make_pair(RPL_TRACECLASS, "Trace class"));
		reply_codes.insert(std::make_pair(RPL_STATSLINKINFO, "Stats link info"));
		reply_codes.insert(std::make_pair(RPL_STATSCOMMANDS, "Stats commands"));
		reply_codes.insert(std::make_pair(RPL_STATSCLINE, "Stats C line"));
		reply_codes.insert(std::make_pair(RPL_STATSNLINE, "Stats N line"));
		reply_codes.insert(std::make_pair(RPL_STATSILINE, "Stats I line"));
		reply_codes.insert(std::make_pair(RPL_STATSKLINE, "Stats K line"));
		reply_codes.insert(std::make_pair(RPL_STATSQLINE, "Stats Q line"));
		reply_codes.insert(std::make_pair(RPL_STATSYLINE, "Stats Y line"));
		reply_codes.insert(std::make_pair(RPL_ENDOFSTATS, "End of stats"));
		reply_codes.insert(std::make_pair(RPL_UMODEIS, "User mode is"));
		reply_codes.insert(std::make_pair(RPL_SERVICEINFO, "Service info"));
		reply_codes.insert(std::make_pair(RPL_ENDOFSERVICES, "End of services"));
		reply_codes.insert(std::make_pair(RPL_SERVICE, "Service"));
		reply_codes.insert(std::make_pair(RPL_SERVLIST, "Service list"));
		reply_codes.insert(std::make_pair(RPL_SERVLISTEND, "Service list end"));
		reply_codes.insert(std::make_pair(RPL_STATSIAUTH, "Stats I auth"));
		reply_codes.insert(std::make_pair(RPL_STATSVLINE, "Stats V line"));
		reply_codes.insert(std::make_pair(RPL_STATSLLINE, "Stats L line"));
		reply_codes.insert(std::make_pair(RPL_STATSUPTIME, "Stats uptime"));
		reply_codes.insert(std::make_pair(RPL_STATSOLINE, "Stats O line"));
		reply_codes.insert(std::make_pair(RPL_STATSHLINE, "Stats H line"));
		reply_codes.insert(std::make_pair(RPL_STATSSLINE, "Stats S line"));
		reply_codes.insert(std::make_pair(RPL_STATSPING, "Stats ping"));
		reply_codes.insert(std::make_pair(RPL_STATSBLINE, "Stats B line"));
		reply_codes.insert(std::make_pair(RPL_STATSDEFINE, "Stats define"));
		reply_codes.insert(std::make_pair(RPL_STATSDEBUG, "Stats debug"));
		reply_codes.insert(std::make_pair(RPL_STATSDLINE, "Stats D line"));
		reply_codes.insert(std::make_pair(RPL_LUSERCLIENT, "Luser client"));
		reply_codes.insert(std::make_pair(RPL_LUSEROP, "Luser operator"));
		reply_codes.insert(std::make_pair(RPL_LUSERUNKNOWN, "Luser unknown"));
		reply_codes.insert(std::make_pair(RPL_LUSERCHANNELS, "Luser channels"));
		reply_codes.insert(std::make_pair(RPL_LUSERME, "Luser me"));
		reply_codes.insert(std::make_pair(RPL_ADMINME, "Admin me"));
		reply_codes.insert(std::make_pair(RPL_ADMINLOC1, "Admin loc1"));
		reply_codes.insert(std::make_pair(RPL_ADMINLOC2, "Admin loc2"));
		reply_codes.insert(std::make_pair(RPL_ADMINEMAIL, "Admin email"));
		reply_codes.insert(std::make_pair(RPL_TRACELOG, "Trace log"));
		reply_codes.insert(std::make_pair(RPL_TRACEEND, "Trace end"));
		reply_codes.insert(std::make_pair(RPL_TRYAGAIN, "Try again"));
		reply_codes.insert(std::make_pair(RPL_LOCALUSERS, "Local users"));
		reply_codes.insert(std::make_pair(RPL_GLOBALUSERS, "Global users"));
		reply_codes.insert(std::make_pair(RPL_NONE, "None"));
		reply_codes.insert(std::make_pair(RPL_AWAY, "Away"));
		reply_codes.insert(std::make_pair(RPL_USERHOST, "User host"));
		reply_codes.insert(std::make_pair(RPL_ISON, "Is on"));
		reply_codes.insert(std::make_pair(RPL_TEXT, "Text"));
		reply_codes.insert(std::make_pair(RPL_UNAWAY, "Unaway"));
		reply_codes.insert(std::make_pair(RPL_NOWAWAY, "Now away"));
		reply_codes.insert(std::make_pair(RPL_WHOISUSER, "Whois user"));
		reply_codes.insert(std::make_pair(RPL_WHOISSERVER, "Whois server"));
		reply_codes.insert(std::make_pair(RPL_WHOISOPERATOR, "Whois operator"));
		reply_codes.insert(std::make_pair(RPL_WHOWASUSER, "Who was user"));
		reply_codes.insert(std::make_pair(RPL_ENDOFWHO, "End of who"));
		reply_codes.insert(std::make_pair(RPL_WHOISCHANOP, "Whois chanop"));
		reply_codes.insert(std::make_pair(RPL_WHOISIDLE, "Whois idle"));
		reply_codes.insert(std::make_pair(RPL_ENDOFWHOIS, "End of whois"));
		reply_codes.insert(std::make_pair(RPL_WHOISCHANNELS, "Whois channels"));
		reply_codes.insert(std::make_pair(RPL_LISTSTART, "List start"));
		reply_codes.insert(std::make_pair(RPL_LIST, "List"));
		reply_codes.insert(std::make_pair(RPL_LISTEND, "List end"));
		reply_codes.insert(std::make_pair(RPL_CHANNELMODEIS, "Channel mode is"));
		reply_codes.insert(std::make_pair(RPL_UNIQOPIS, "Unique op is"));
		reply_codes.insert(std::make_pair(RPL_NOTOPIC, "No topic"));
		reply_codes.insert(std::make_pair(RPL_TOPIC, "Topic"));
		reply_codes.insert(std::make_pair(RPL_TOPIC_WHO_TIME, "Topic who time"));
		reply_codes.insert(std::make_pair(RPL_INVITING, "Inviting"));
		reply_codes.insert(std::make_pair(RPL_SUMMONING, "Summoning"));
		reply_codes.insert(std::make_pair(RPL_REOPLIST, "Reop list"));
		reply_codes.insert(std::make_pair(RPL_ENDOFREOPLIST, "End of reop list"));
		reply_codes.insert(std::make_pair(RPL_INVITELIST, "Invite list"));
		reply_codes.insert(std::make_pair(RPL_ENDOFINVITELIST, "End of invite list"));
		reply_codes.insert(std::make_pair(RPL_EXCEPTLIST, "Except list"));
		reply_codes.insert(std::make_pair(RPL_VERSION, "Version"));
		reply_codes.insert(std::make_pair(RPL_ENDOFEXCEPTLIST, "End of except list"));
		reply_codes.insert(std::make_pair(RPL_WHOREPLY, "Who reply"));
		reply_codes.insert(std::make_pair(RPL_NAMREPLY, "Name reply"));
		reply_codes.insert(std::make_pair(RPL_KILLDONE, "Kill done"));
		reply_codes.insert(std::make_pair(RPL_CLOSING, "Closing"));
		reply_codes.insert(std::make_pair(RPL_CLOSEEND, "Close end"));
		reply_codes.insert(std::make_pair(RPL_LINKS, "Links"));
		reply_codes.insert(std::make_pair(RPL_ENDOFLINKS, "End of links"));
		reply_codes.insert(std::make_pair(RPL_ENDOFNAMES, "End of names"));
		reply_codes.insert(std::make_pair(RPL_BANLIST, "Ban list"));
		reply_codes.insert(std::make_pair(RPL_ENDOFBANLIST, "End of ban list"));
		reply_codes.insert(std::make_pair(RPL_ENDOFWHOWAS, "End of who was"));
		reply_codes.insert(std::make_pair(RPL_INFO, "Info"));
		reply_codes.insert(std::make_pair(RPL_MOTD, "Motd"));
		reply_codes.insert(std::make_pair(RPL_INFOSTART, "Info start"));
		reply_codes.insert(std::make_pair(RPL_ENDOFINFO, "End of info"));
		reply_codes.insert(std::make_pair(RPL_MOTDSTART, "Motd start"));
		reply_codes.insert(std::make_pair(RPL_ENDOFMOTD, "End of motd"));
		reply_codes.insert(std::make_pair(RPL_YOUREOPER, "You are oper"));
		reply_codes.insert(std::make_pair(RPL_REHASHING, "Rehashing"));
		reply_codes.insert(std::make_pair(RPL_YOURESERVICE, "You are service"));
		reply_codes.insert(std::make_pair(RPL_MYPORTIS, "My port is"));
		reply_codes.insert(std::make_pair(RPL_NOTOPERANYMORE, "Not oper anymore"));
		reply_codes.insert(std::make_pair(RPL_TIME, "Time"));
		reply_codes.insert(std::make_pair(RPL_USERSSTART, "Users start"));
		reply_codes.insert(std::make_pair(RPL_USERS, "Users"));
		reply_codes.insert(std::make_pair(RPL_ENDOFUSERS, "End of users"));
		reply_codes.insert(std::make_pair(RPL_NOUSERS, "No users"));
		reply_codes.insert(std::make_pair(ERR_NOSUCHNICK, "No such nick"));
		reply_codes.insert(std::make_pair(ERR_NOSUCHSERVER, "No such server"));
		reply_codes.insert(std::make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
		reply_codes.insert(std::make_pair(ERR_CANNOTSENDTOCHAN, "Cannot send to chan"));
		reply_codes.insert(std::make_pair(ERR_TOOMANYCHANNELS, "Too many channels"));
		reply_codes.insert(std::make_pair(ERR_WASNOSUCHNICK, "Was no such nick"));
		reply_codes.insert(std::make_pair(ERR_TOOMANYTARGETS, "Too many targets"));
		reply_codes.insert(std::make_pair(ERR_NOSUCHSERVICE, "No such service"));
		reply_codes.insert(std::make_pair(ERR_NOORIGIN, "No origin"));
		reply_codes.insert(std::make_pair(ERR_NORECIPIENT, "No recipient"));
		reply_codes.insert(std::make_pair(ERR_NOTEXTTOSEND, "No text to send"));
		reply_codes.insert(std::make_pair(ERR_NOTOPLEVEL, "No top level"));
		reply_codes.insert(std::make_pair(ERR_WILDTOPLEVEL, "Wild top level"));
		reply_codes.insert(std::make_pair(ERR_BADMASK, "Bad mask"));
		reply_codes.insert(std::make_pair(ERR_TOOMANYMATCHES, "Too many matches"));
		reply_codes.insert(std::make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
		reply_codes.insert(std::make_pair(ERR_NOMOTD, "No motd"));
		reply_codes.insert(std::make_pair(ERR_NOADMININFO, "No admin info"));
		reply_codes.insert(std::make_pair(ERR_FILEERROR, "File error"));
		reply_codes.insert(std::make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
		reply_codes.insert(std::make_pair(ERR_ERRONEOUSNICKNAME, "Erroneous nickname"));
		reply_codes.insert(std::make_pair(ERR_NICKNAMEINUSE, "Nickname in use"));
		reply_codes.insert(std::make_pair(ERR_SERVICENAMEINUSE, "Service name in use"));
		reply_codes.insert(std::make_pair(ERR_SERVICECONFUSED, "Service confused"));
		reply_codes.insert(std::make_pair(ERR_NICKCOLLISION, "Nick collision"));
		reply_codes.insert(std::make_pair(ERR_UNAVAILRESOURCE, "Unavailable resource"));
		reply_codes.insert(std::make_pair(ERR_USERNOTINCHANNEL, "User not in channel"));
		reply_codes.insert(std::make_pair(ERR_NOTONCHANNEL, "Not on channel"));
		reply_codes.insert(std::make_pair(ERR_USERONCHANNEL, "User on channel"));
		reply_codes.insert(std::make_pair(ERR_NOLOGIN, "No login"));
		reply_codes.insert(std::make_pair(ERR_SUMMONDISABLED, "Summon disabled"));
		reply_codes.insert(std::make_pair(ERR_USERSDISABLED, "Users disabled"));
		reply_codes.insert(std::make_pair(ERR_NOTREGISTERED, "Not registered"));
		reply_codes.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Need more params"));
		reply_codes.insert(std::make_pair(ERR_ALREADYREGISTRED, "Already registered"));
		reply_codes.insert(std::make_pair(ERR_NOPERMFORHOST, "No perm for host"));
		reply_codes.insert(std::make_pair(ERR_PASSWDMISMATCH, "Password mismatch"));
		reply_codes.insert(std::make_pair(ERR_YOUREBANNEDCREEP, "You're banned creep"));
		reply_codes.insert(std::make_pair(ERR_YOUWILLBEBANNED, "You will be banned"));
		reply_codes.insert(std::make_pair(ERR_KEYSET, "Key set"));
		reply_codes.insert(std::make_pair(ERR_CHANNELISFULL, "Channel is full"));
		reply_codes.insert(std::make_pair(ERR_UNKNOWNMODE, "Unknown mode"));
		reply_codes.insert(std::make_pair(ERR_INVITEONLYCHAN, "Invite only chan"));
		reply_codes.insert(std::make_pair(ERR_BANNEDFROMCHAN, "Banned from chan"));
		reply_codes.insert(std::make_pair(ERR_BADCHANNELKEY, "Bad channel key"));
		reply_codes.insert(std::make_pair(ERR_BADCHANMASK, "Bad chan mask"));
		reply_codes.insert(std::make_pair(ERR_NOCHANMODES, "No chan modes"));
		reply_codes.insert(std::make_pair(ERR_BANLISTFULL, "Ban list full"));
		reply_codes.insert(std::make_pair(ERR_NOPRIVILEGES, "No privileges"));
		reply_codes.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, "Chanop privileges needed"));
		reply_codes.insert(std::make_pair(ERR_CANTKILLSERVER, "Can't kill server"));
		reply_codes.insert(std::make_pair(ERR_RESTRICTED, "Restricted"));
		reply_codes.insert(std::make_pair(ERR_UNIQOPRIVSNEEDED, "Unique op privileges needed"));
		reply_codes.insert(std::make_pair(ERR_NOOPERHOST, "No oper host"));
		reply_codes.insert(std::make_pair(ERR_NOSERVICEHOST, "No service host"));
		reply_codes.insert(std::make_pair(ERR_STATSKLINE, "Stats K line"));
		reply_codes.insert(std::make_pair(ERR_UMODEUNKNOWNFLAG, "Umode unknown flag"));
		reply_codes.insert(std::make_pair(ERR_USERSDONTMATCH, "Users don't match"));
		reply_codes.insert(std::make_pair(RPL_ETRACEFULL, "Etrace full"));
		reply_codes.insert(std::make_pair(RPL_ETRACEEND, "Etrace end"));
		return reply_codes;
	}

    const std::map<uint16_t, std::string> reply_codes = create_reply_codes();
}
