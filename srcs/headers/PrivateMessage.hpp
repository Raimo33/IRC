/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:03:42 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:24:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

# include "IMessage.hpp"
# include "irc.hpp"

using namespace irc;

class PrivateMessage: public IMessage
{
	public:
		explicit PrivateMessage(const string &content, const Client &sender, const Client &receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage(void);

		const Client	*getReceiver(void) const;

	private:
		const Client	*_receiver;
};

#endif