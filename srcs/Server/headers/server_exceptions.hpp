/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exceptions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 17:49:27 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_EXCEPTIONS_HPP
#define SERVER_EXCEPTIONS_HPP

# include "ReplyMessage.hpp"
# include "common_exceptions.hpp"

#include <exception>

class ProtocolErrorException : public std::exception // for IRC protocol errors (reply codes)
{
public:
	explicit ProtocolErrorException(const int reply_code, ...);
	const char *what(void) const throw();
	~ProtocolErrorException(void) throw();

	ReplyMessage &getContent(void);

private:
	ReplyMessage _content;
	const std::string _formatted_msg;
};

#endif