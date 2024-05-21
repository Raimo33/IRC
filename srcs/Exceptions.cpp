/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:09:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Exceptions.hpp"

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

const char *InvalidArgumentException::what(void) const throw()
{
	return invalid_argument::what();
}

const char *Server::ChannelAlreadyExistsException::what(void) const throw()
{
	return "Channel already exists";
}

const char *Server::ChannelNotFoundException::what(void) const throw()
{
	return "Channel not found";
}

const char *Server::InvalidPasswordException::what(void) const throw()
{
	return "Invalid password";
}

const char *Server::ClientNotFoundException::what(void) const throw()
{
	return "Client not found";
}

const char *Server::ClientAlreadyExistsException::what(void) const throw()
{
	return "Client already exists";
}

const char *Server::HandshakeFailedException::what(void) const throw()
{
	return "Handshake failed";
}

const char	*EventHandler::UnkownCommandExcetption::what(void) const throw()
{
	return "Command not found";
}

const char	*EventHandler::CantSendMessageToYourselfException::what(void) const throw()
{
	return "You can't send a message to yourself";
}

const char	*Channel::InvalidNameException::what(void) const throw()
{
	return "Invalid channel name";
}

const char	*Channel::InvalidTopicException::what(void) const throw()
{
	return "Invalid channel topic";
}

const char	*Channel::InvalidKeyException::what(void) const throw()
{
	return "Invalid channel key";
}

const char	*Channel::ChannelFullException::what(void) const throw()
{
	return "Channel is full";
}

const char	*Channel::UserAlreadyMemberException::what(void) const throw()
{
	return "Client is already a member of the channel";
}

const char	*Channel::UserAlreadyOperatorException::what(void) const throw()
{
	return "Client is already an operator of the channel";
}

const char	*Channel::UserNotOperatorException::what(void) const throw()
{
	return "Client is not an operator of the channel";
}

const char	*Channel::UserNotMemberException::what(void) const throw()

{
	return "Client is not a member of the channel";
}

const char	*Channel::UnknownModeException::what(void) const throw()
{
	return "Unknown channel mode";
}

const char	*Client::TooManyChannelsException::what(void) const throw()
{
	return "User are already in the maximum number of channels allowed";
}

const char	*Client::AlreadyConnectedException::what(void) const throw()
{
	return "User are already connected";
}

const char	*Client::NotConnectedException::what(void) const throw()
{
	return "User are not connected";
}

const char	*Client::AlreadyAuthenticatedException::what(void) const throw()
{
	return "User are already authenticated";
}

const char	*Client::NotAuthenticatedException::what(void) const throw()
{
	return "User are not authenticated";
}

const char	*Client::NicknameInUseException::what(void) const throw()
{
	return "User nickname is already in use";
}

const char	*Client::ErroneousNicknameException::what(void) const throw()
{
	return "User nickname is invalid";
}

const char	*Client::UserNotInChannelException::what(void) const throw()
{
	return "User is not in the channel";
}

const char	*Client::CantSendMessageToYourselfException::what(void) const throw()
{
	return "User can't send a message to himself";
}
