/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 21:57:11 by craimond         ###   ########.fr       */
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
	public:
		explicit ProtocolErrorException(const uint16_t code, ...);
		virtual const char* what(void) const throw();
		~ProtocolErrorException(void) throw();

		uint16_t						getCode(void) const;
		const std::vector<std::string>&	getParams(void) const;
	private:

		uint16_t						_code;
   		std::vector<std::string>		_params;
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