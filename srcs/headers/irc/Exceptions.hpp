/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/30 20:22:30 by craimond         ###   ########.fr       */
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

# include <irc/Messages.hpp>

class ProtocolErrorException : public std::exception //for IRC protocol errors (reply codes)
{
	public:
		explicit ProtocolErrorException(const int code, ...);
		const char *what(void) const throw();
		~ProtocolErrorException(void) throw();

		const struct s_message	&getContent(void) const;

	private:	
		struct s_message		_content;
		const std::string		_formatted_msg;
};

class InternalErrorException : public std::runtime_error //for internal programming errors such as null pointers, calling addClient 2 times on the same client, etc.
{
	public:
		InternalErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

class SystemErrorException : public std::runtime_error //for system calls errors (unpredictable)
{
	public:
		SystemErrorException(const int errnum);
		SystemErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

#endif