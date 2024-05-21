/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Exceptions.hpp"

using namespace std;
using namespace irc;

SystemErrorException::SystemErrorException(const string &msg) : runtime_error(msg) {}

const char *SystemErrorException::what(void) const throw()
{
	return runtime_error::what();
}

FatalErrorException::FatalErrorException(const string &msg) : runtime_error(msg) {}

const char *FatalErrorException::what(void) const throw()
{
	return runtime_error::what();
}

InvalidArgumentException::InvalidArgumentException(const string &msg) : invalid_argument(msg) {}

const char *AlreadyAuthenticatedException::what(void) const throw()
{
    return "User is already authenticated";
}

const char *AlreadyConnectedException::what(void) const throw()
{
    return "User is already connected";
}

const char *CantSendMessageToYourselfException::what(void) const throw()
{
    return "You can't send a message to yourself";
}

const char *ChannelAlreadyExistsException::what(void) const throw()
{
    return "Channel already exists";
}

const char *ChannelFullException::what(void) const throw()
{
    return "Channel is full";
}

const char *ChannelNotFoundException::what(void) const throw()
{
    return "Channel not found";
}

const char *ClientAlreadyExistsException::what(void) const throw()
{
    return "Client already exists";
}

const char *ClientNotFoundException::what(void) const throw()
{
    return "Client not found";
}

const char *ErroneousNicknameException::what(void) const throw()
{
    return "User nickname is invalid";
}

const char *HandshakeFailedException::what(void) const throw()
{
    return "Handshake failed";
}

const char *InvalidArgumentException::what(void) const throw()
{
    return std::invalid_argument::what();
}

const char *InvalidKeyException::what(void) const throw()
{
    return "Invalid channel key";
}

const char *InvalidNameException::what(void) const throw()
{
    return "Invalid channel name";
}

const char *InvalidPasswordException::what(void) const throw()
{
    return "Invalid password";
}

const char *InvalidTopicException::what(void) const throw()
{
    return "Invalid channel topic";
}

const char *NicknameInUseException::what(void) const throw()
{
    return "User nickname is already in use";
}

const char *NotAuthenticatedException::what(void) const throw()
{
    return "User is not authenticated";
}

const char *NotConnectedException::what(void) const throw()
{
    return "User is not connected";
}

const char *OperatorNotInChannelException::what(void) const throw()
{
    return "Operator is not in the channel";
}

const char *TooManyChannelsException::what(void) const throw()
{
    return "User is already in the maximum number of channels allowed";
}

const char *UnknownModeException::what(void) const throw()
{
    return "Unknown channel mode";
}

const char *UnknownCommandException::what(void) const throw()
{
    return "Command not found";
}

const char *UserAlreadyMemberException::what(void) const throw()
{
    return "Client is already a member of the channel";
}

const char *UserAlreadyOperatorException::what(void) const throw()
{
    return "Client is already an operator of the channel";
}

const char *UserNotMemberException::what(void) const throw()
{
    return "Client is not a member of the channel";
}

const char *UserNotOperatorException::what(void) const throw()
{
    return "Client is not an operator of the channel";
}

