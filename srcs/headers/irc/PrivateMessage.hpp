/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 15:29:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

# include "AMessage.hpp"

namespace irc
{
	class Client;

	class PrivateMessage: public AMessage
	{
		public:
			explicit PrivateMessage(const std::string &text, const Client &sender, const Client &receiver);
			PrivateMessage(const PrivateMessage &copy);
			~PrivateMessage(void);

			const Client	*getReceiver(void) const;

		private:
			const Client	*_receiver;
	};
}

#endif