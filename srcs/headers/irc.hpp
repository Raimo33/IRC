/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:00:53 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 16:21:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <exception>
# include <stdexcept>

namespace irc
{
	class Server;
	class Client;
	class ChannelOperator;
	class Channel;
	class EventHandler;
	class IMessage;
	class Message;
	class PrivateMessage;
	class MD5;

	class IRCException;
	class SystemErrorException;
	class FatalErrorException;
	class InvalidArgumentException;
	class ChannelAlreadyExistsException;
	class ChannelNotFoundException;
	class InvalidPasswordException;
	class ClientNotFoundException;
	class ClientAlreadyExistsException;
	class HandshakeFailedException;
	class UnkownCommandExcetption;
	class InvalidNameException;
	class InvalidTopicException;
	class InvalidKeyException;
	class ChannelFullException;
	class UserAlreadyMemberException;
	class UserAlreadyOperatorException;
	class UserNotOperatorException;
	class UserNotMemberException;
	class UnknownModeException;
	class TooManyChannelsException;
	class AlreadyConnectedException;
	class NotConnectedException;
	class AlreadyAuthenticatedException;
	class NotAuthenticatedException;
	class NicknameInUseException;
	class ErroneousNicknameException;
	class UserNotInChannelException;
	class CantSendMessageToYourselfException;
}

#endif