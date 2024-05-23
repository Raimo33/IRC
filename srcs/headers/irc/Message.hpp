/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:57:26 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 15:29:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

# include "AMessage.hpp"

//quando si crea l'oggetto messaggio bisogna controllare se _channel.size() e' uguale a 2
//e in quel caso si promuove (col costruttore copy) a private message

namespace irc
{
	class Channel;

	class Message : public AMessage
	{
		public:
			//un messaggio in un canale di 2 persone deve passare come private message
			explicit Message(const std::string &text, const Client &sender, const Channel &channel);
			Message(const Message &copy);
			~Message(void);

			const Channel	*getChannel(void) const;

		private:
			const Channel	*_channel;
	};
}

//ad ogni messaggio corrisponde una risposta numerica (vedi RFC)
//https://datatracker.ietf.org/doc/html/rfc1459#section-6.1

#endif