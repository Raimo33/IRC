/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:57:26 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 18:09:31 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

# include "IMessage.hpp"

using namespace std;

class User;
class Channel;

//quando si crea l'oggetto messaggio bisogna controllare se _channel.size() e' uguale a 2
//e in quel caso si promuove (col costruttore copy) a private message
class Message : public IMessage
{
	public:
		Message();
		//un messaggio in un canale di 2 persone deve passare come private message
		Message(const string &content, const User &sender, const Channel &channel);
		Message(const Message &copy);
		~Message();
		Channel	&getChannel() const;
		void	setChannel(Channel &channel);
	private:
		Channel	*_channel;
};

//ad ogni messaggio corrisponde una risposta numerica (vedi RFC)
//https://datatracker.ietf.org/doc/html/rfc1459#section-6.1

#endif