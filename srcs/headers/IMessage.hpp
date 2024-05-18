/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:12 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 14:55:22 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <string>

using namespace std;

class User;

class IMessage
{
	public:
		explicit IMessage(const string &content, User &sender);
		IMessage(const IMessage &copy);
		virtual ~IMessage(void) = 0;

		const string getContent(void) const;
		const time_t getTimestamp(void) const;
		const User &getSender(void) const;

	protected:
		const string _content;
		const time_t _timestamp;
		const User *_sender;
};

#endif