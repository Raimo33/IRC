/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 16:21:57 by craimond         ###   ########.fr       */
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
		IMessage(void);
		IMessage(const string &content, User &sender);
		virtual ~IMessage(void) = 0;
		User	&getSender(void) const;
		string	getContent(void) const;
		time_t	getTimestamp(void) const;
		void	setContent(const string &content);
		void	setTimestamp(const time_t timestamp);
		void	setSender(User &sender);
	protected:
		string	_content;
		time_t	_timestamp;
		User	*_sender;
};

#endif