/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_exceptions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 17:05:01 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include "ReplyMessage.hpp"

# include <string>
# include <exception>
# include <stdexcept>
# include <cstdarg>
# include <vector>
# include <stdint.h>

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