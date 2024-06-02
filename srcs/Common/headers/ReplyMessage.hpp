/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 18:36:44 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 15:36:58 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLYMESSAGE_HPP
# define REPLYMESSAGE_HPP

#include "AMessage.hpp"

#include <map>
#include <string>

//https://github.com/williamkapke/irc-replies/blob/master/replies.json

enum e_replyCodes
{
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_YOUREOPER = 381,
	RPL_NOTOPERANYMORE = 385,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYCHANNELS = 405,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_FILEERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEOUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_NOCHANMODES = 477,
	ERR_CHANOPRIVSNEEDED = 482,
};

class ReplyMessage : public AMessage
{
	public:
		ReplyMessage(void);
		ReplyMessage(const std::string &raw_input);
		ReplyMessage(const std::string &prefix, const enum e_replyCodes code, ...);
		ReplyMessage(const std::string &prefix, const int value, va_list args);
		ReplyMessage(const ReplyMessage &copy);
		~ReplyMessage(void);
	
		ReplyMessage		&operator=(const ReplyMessage &copy);
		
		void				setReplyCode(const enum e_replyCodes code);
		enum e_replyCodes	getReplyCode(void) const;
	
	private:
		void				parse(std::string raw_input);

		enum e_replyCodes	_reply_code;
};

const std::map<enum e_replyCodes, const char *>			create_default_replies_map(void); //TODO mettere statici dentro la classe
extern const std::map<enum e_replyCodes, const char *>	g_default_replies;

#endif