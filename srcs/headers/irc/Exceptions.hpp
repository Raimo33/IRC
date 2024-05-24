/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 12:29:26 by craimond         ###   ########.fr       */
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

# include <irc/Content.hpp>

namespace irc
{
	class SystemErrorException; //for system calls errors (unpredictable)
	class InternalErrorException; //for internal programming errors such as null pointers, calling addClient 2 times on the same client, etc.
	class ProtocolErrorException; //for IRC protocol errors (reply codes)
}

class irc::ProtocolErrorException : public std::exception
{
	public:
		explicit ProtocolErrorException(const std::string &custom_msg, const uint32_t code, ...);
		~ProtocolErrorException(void) throw();

		const struct s_replyContent	&getContent(void) const;

	private:
		struct s_replyContent	_content;
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