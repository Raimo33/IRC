/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:51:58 by egualand          #+#    #+#             */
/*   Updated: 2024/07/10 14:32:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_UTILS_HPP
# define SERVER_UTILS_HPP

# include <string>
# include <stdint.h>

#include "common_utils.hpp"

bool		is_channel_prefix(const char c);
bool		is_valid_channel_name(const std::string &name);
bool		is_valid_channel_key(const std::string &key);
bool		is_valid_channel_mode(const char mode);
bool		is_valid_nickname(const std::string &name);
bool		channel_mode_requires_param(const char mode, const bool status);
std::string	hash(const std::string &s);
uint64_t	ip_to_long(const std::string &ip);
bool		is_upper_finnish(const char c);
std::string	to_lower_finnish(const std::string &s);

#endif