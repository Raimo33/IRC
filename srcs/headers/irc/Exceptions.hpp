/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:44:51 by craimond         ###   ########.fr       */
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
		explicit ProtocolErrorException(const enum e_replyCodes code, const std::string params[] = NULL, const std::string &custom_msg = "");
		explicit ProtocolErrorException(const enum e_replyCodes code, const std::string &param, const std::string &custom_msg = "");
		const char *what(void) const throw();
		~ProtocolErrorException(void) throw();

		const struct s_replyMessage	&getContent(void) const;

	private:
		const struct s_replyMessage	_content;
		const std::string			_formatted_msg;
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
		SystemErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

#endif