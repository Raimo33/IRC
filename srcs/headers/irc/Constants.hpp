/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 03:05:20 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 17:49:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

# include <stdint.h>

const char *const	SERVER_NAME = "IRCServer";
const char *const	SERVER_VERSION = "1.0";
const uint8_t		MAX_CHANNELS_PER_USER = 10;
const uint8_t		MAX_NICKNAME_LEN = 9;
const uint16_t		MAX_CHANNEL_NAME_LEN = 200;
const uint16_t		MAX_CHANNEL_TOPIC_LEN = 390;
const uint8_t		MAX_CHANNEL_KEY_LEN = 50;
const uint16_t		BUFFER_SIZE = 512;
const uint8_t		MAX_SERVER_PASSWORD_LEN = 64;
const uint8_t		N_COMMANDS = 11;
const char *const	SUPPORTED_CHANNEL_MODES = "itkol";
const char *const	LOG_FILENAME = "IRCServer.log";

#endif