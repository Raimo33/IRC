/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 16:24:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <string>
# include <exception>
# include <stdexcept>

namespace irc
{
	class SystemErrorException; //for system calls errors (unpredictable)
	class InternalErrorException; //for internal programming errors such as setters
	class ProtocolErrorException; //for IRC protocol errors
}

class irc::ProtocolErrorException : public std::exception
{
	public:
		explicit ProtocolErrorException(const uint16_t code, const std::vector<std::string> &params);
		virtual const char* what(void) const throw();
		~ProtocolErrorException(void) throw();

		uint16_t						getCode(void) const;
		const std::vector<std::string>&	getParams(void) const;
	private:
		static std::string generateMessage(const uint16_t code, const std::vector<std::string>& params);

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