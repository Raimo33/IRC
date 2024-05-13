/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 17:22:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
# define IMESSAGE_HPP

# include <string>

using namespace std;

class User;

class IMessage
{
	public:
		IMessage();
		virtual ~IMessage() = 0;
		User	&getSender() const;
		string	&getContent() const;
		time_t	getTimestamp() const;
		void	setContent(const string &content);
		void	setTimestamp(const time_t timestamp);
		void	setSender(const User &sender);
	protected:
		const string		_content;
		const time_t		_timestamp;
};

#endif