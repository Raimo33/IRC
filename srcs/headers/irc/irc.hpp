/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:00:53 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 22:39:46 by craimond         ###   ########.fr       */
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
	class Hasher;

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

# include "irc/Server.hpp"
# include "irc/Client.hpp"
# include "irc/ChannelOperator.hpp"
# include "irc/Channel.hpp"
# include "irc/EventHandler.hpp"
# include "irc/IMessage.hpp"
# include "irc/Message.hpp"
# include "irc/PrivateMessage.hpp"
# include "irc/Hasher.hpp"
# include "irc/Exceptions.hpp"

#endif