/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:57:26 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 13:04:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>

using namespace std;

class User;

class Message
{
	public:
		Message();
		Message(const string &content, const User *sender);
		Message(const Message &copy);
		~Message();
		string		getContent() const;
		time_t		getTimestamp() const;
		const User	*getSender() const;

	private:
		const string	_content;
		const time_t	_timestamp;
		const User		*_sender;
};

#endif