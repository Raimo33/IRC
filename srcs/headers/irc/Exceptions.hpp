/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/23 14:02:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <string>
# include <exception>
# include <stdexcept>
# include <cstdarg>
# include <vector>
# include <stdint.h>

namespace irc
{
	class SystemErrorException; //for system calls errors (unpredictable)
	class InternalErrorException; //for internal programming errors such as null pointers, calling addClient 2 times on the same client, etc.
	class ProtocolErrorException; //for IRC protocol errors (reply codes)
}

class irc::ProtocolErrorException : public std::exception
{
	//TODO far costruire direttamente a lui la struct s_replyMessage
	public:
		explicit ProtocolErrorException(const uint16_t code, ...);
		virtual const char* what(void) const throw();
		~ProtocolErrorException(void) throw();

		struct s_replyMessage	&getContent(void) const;
	private:

		struct s_replyMessage	_content;
};

class irc::InternalErrorException : public std::runtime_error
{
	public:
		InternalErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

class irc::SystemErrorException : public std::runtime_error
{
	public:
		SystemErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

#endif