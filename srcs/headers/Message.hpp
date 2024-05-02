/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:57:26 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 16:23:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>

using namespace std;

class User;
class Channel;

/*
IRC messages are always lines of characters terminated with a CR-LF
   (Carriage Return - Line Feed) pair, and these messages shall not
   exceed 512 characters in length, counting all characters including
   the trailing CR-LF
  	<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
*/

struct t_msg
{
	string		prefix;
	string		command;
	string		params;
};

//quando si crea l'oggetto messaggio bisogna controllare se _channel.size() e' uguale a 2
//e in quel caso si promuove (col costruttore copy) a private message
class Message
{
	public:
		Message();
		//un messaggio in un canale di 2 persone deve passare come private message
		Message(const string &content, const User &sender, const Channel &channel);
		Message(const Message &copy);
		~Message();
		string		getContent() const;
		time_t		getTimestamp() const;
		const User	*getSender() const;

	private:
		const struct t_msg	_content;
		const time_t		_timestamp;
		const User			*_sender;
		const Channel		*_channel;
};

//ad ogni messaggio corrisponde una risposta numerica (vedi RFC)
//https://datatracker.ietf.org/doc/html/rfc1459#section-6.1

#endif