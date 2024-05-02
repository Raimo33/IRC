/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 13:12:28 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 13:12:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

# include "Message.hpp"

using namespace std;

class PrivateMessage : public Message
{
	public:
		PrivateMessage();
		PrivateMessage(const string &content, const User *sender, const User *receiver);
		PrivateMessage(const PrivateMessage &copy);
		~PrivateMessage();
		const User	*getReceiver() const;

	private:
		const User	*_receiver;
};

#endif