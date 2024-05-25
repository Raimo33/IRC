/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:57:26 by craimond          #+#    #+#             */
/*   Updated: 2024/05/25 15:42:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

# include "AMessage.hpp"

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

#endif