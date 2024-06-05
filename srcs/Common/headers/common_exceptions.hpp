/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_exceptions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 18:12:57 by craimond         ###   ########.fr       */
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

//for internal programming errors such as null pointers, calling addClient 2 times on the same client, etc.
class InternalErrorException : public std::runtime_error
{
	public:
		InternalErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

//for system calls errors (unpredictable)
class SystemErrorException : public std::runtime_error
{
	public:
		SystemErrorException(const int errnum);
		SystemErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

#endif