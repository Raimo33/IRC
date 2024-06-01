/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:38:24 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 11:08:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENT_HPP
# define CONTENT_HPP

# include <string>
# include <vector>
# include <map>
# include <cstdarg>
# include <stdint.h>

enum e_commands
{
	PASS = 1001,
	NICK = 1002,
	USER = 1003,
	JOIN = 1004,
	PART = 1005,
	PRIVMSG = 1006,
	QUIT = 1007,
	SEND = 1008,
	KICK = 1009,
	INVITE = 1010,
	TOPIC = 1011,
	MODE = 1012,
};

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

class Message
{
	public:
		Message(void);
		Message(const std::string &raw_input);
		Message(const std::string &prefix, const int value, ...);
		Message(const std::string &prefix, const int value, va_list args);
		Message(const Message &copy);
		~Message(void);

		Message	&operator=(const Message &copy);

		const std::string							&getPrefix(void) const;
		void										setPrefix(const std::string &prefix);
		uint16_t									getValue(void) const;
		void										setValue(const uint16_t value);
		const std::vector<std::string>				&getParams(void) const;
		void										setParams(const std::vector<std::string> &params);
		void										setParam(const std::string &param, const int32_t index = -1);
		const std::map<std::string, e_commands>		&getCommands(void) const;
		void										setCommands(const std::map<std::string, e_commands> &commands);

	private:
		void										initCommands(void);
		void										parse(std::string raw_input);

		std::string									_prefix;
		uint16_t									_value;
		std::vector<std::string>					_params;
		std::map<std::string, e_commands>			_commands;
};

const std::map<enum e_replyCodes, const char *>			create_default_replies(void);
extern const std::map<enum e_replyCodes, const char *>	default_replies;

#endif