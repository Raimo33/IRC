/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyCodes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:57:53 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:36:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLYCODES_HPP
# define REPLYCODES_HPP

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

# include <map>
# include <string>
# include <stdint.h>

const std::map<enum e_replyCodes, std::string>			create_reply_codes(void);
extern const std::map<enum e_replyCodes, std::string>	reply_codes;

#endif