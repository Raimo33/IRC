/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:08:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 03:39:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <string>
# include <exception>
# include <stdexcept>

namespace irc
{
	class IRCException;
	class SystemErrorException;
	class FatalErrorException;
	
	class AlreadyAuthenticatedException;
	class AlreadyConnectedException;
	class CantSendMessageToYourselfException;
	class ChannelAlreadyExistsException;
	class ChannelFullException;
	class ChannelNotFoundException;
	class ClientAlreadyExistsException;
	class ClientNotFoundException;
	class ErroneousNicknameException;
	class InvalidArgumentException;
	class InvalidKeyException;
	class InvalidNameException;
	class InvalidPasswordException;
	class InvalidTopicException;
	class NicknameInUseException;
	class NotAuthenticatedException;
	class NotConnectedException;
	class OperatorNotInChannelException;
	class TooManyChannelsException;
	class UnknownCommandException;
	class UnknownModeException;
	class UserAlreadyMemberException;
	class UserAlreadyOperatorException;
	class UserNotMemberException;
	class UserNotOperatorException;
}

class irc::IRCException : public std::exception
{
	public:
		virtual const char *what(void) const throw() = 0;
};

class irc::SystemErrorException : public std::runtime_error
{
	public:
		SystemErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

class irc::FatalErrorException : public std::runtime_error
{
	public:
		FatalErrorException(const std::string &msg);
		virtual const char *what(void) const throw();
};

class irc::AlreadyAuthenticatedException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::AlreadyConnectedException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::CantSendMessageToYourselfException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ChannelAlreadyExistsException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ChannelFullException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ChannelNotFoundException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ClientAlreadyExistsException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ClientNotFoundException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::ErroneousNicknameException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::InvalidArgumentException : public std::invalid_argument
{
	public:
		InvalidArgumentException(const std::string &msg);
		virtual const char *what(void) const throw();
};

class irc::InvalidKeyException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::InvalidNameException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::InvalidPasswordException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::InvalidTopicException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::NicknameInUseException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::NotAuthenticatedException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::NotConnectedException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::OperatorNotInChannelException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::TooManyChannelsException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};


class irc::UnknownCommandException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::UnknownModeException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::UserAlreadyMemberException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::UserAlreadyOperatorException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::UserNotMemberException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

class irc::UserNotOperatorException : public irc::IRCException
{
	public:
		virtual const char *what(void) const throw();
};

//TODO chiedere a chatgpt se vede alcuni doppioni

#endif