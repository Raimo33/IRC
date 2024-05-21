/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:23:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <exception>
# include <stdexcept>

# include "irc.hpp"

using namespace std;
using namespace irc;

class IRCException : public exception
{
	public:
		virtual const char	*what(void) const throw() = 0;
};

class SystemErrorException : public runtime_error, public IRCException
{
	public:
		explicit SystemErrorException(const string &msg);
		const char	*what(void) const throw();
};

class FatalErrorException : public runtime_error, public IRCException
{
	public:
		explicit FatalErrorException(const string &msg);
		const char	*what(void) const throw();
};

class InvalidArgumentException : public invalid_argument, public IRCException
{
	public:
		explicit InvalidArgumentException(const string &msg);
		const char	*what(void) const throw();
};

class ChannelAlreadyExistsException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class ChannelNotFoundException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class InvalidPasswordException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class ClientNotFoundException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class ClientAlreadyExistsException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class HandshakeFailedException : public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UnkownCommandExcetption : public IRCException
{
	public:
		const char	*what(void) const throw();
};

class CantSendMessageToYourselfException : public IRCException
{
	public:
		const char	*what(void) const throw();
};

class InvalidNameException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class InvalidTopicException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class InvalidKeyException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class ChannelFullException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UserAlreadyMemberException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UserAlreadyOperatorException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UserNotOperatorException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UserNotMemberException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class UnknownModeException: public IRCException
{
	public:
		virtual const char *what(void) const throw();
};


class TooManyChannelsException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class AlreadyConnectedException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class NotConnectedException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class AlreadyAuthenticatedException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class NotAuthenticatedException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class NicknameInUseException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class ErroneousNicknameException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class UserNotInChannelException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

class CantSendMessageToYourselfException : public IRCException
{
	public:
		virtual const char	*what(void) const throw();
};

#endif