/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exceptions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 18:17:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_EXCEPTIONS_HPP
#define SERVER_EXCEPTIONS_HPP

# include "ReplyMessage.hpp"
# include "common_exceptions.hpp"

#include <exception>

class ActionFailedException : public std::exception // for IRC protocol errors (reply codes)
{
public:
	explicit ActionFailedException(const int reply_code, ...);
	const char *what(void) const throw();
	~ActionFailedException(void) throw();

	ReplyMessage &getContent(void);

private:
	ReplyMessage _content;
	const std::string _formatted_msg;
};

#endif