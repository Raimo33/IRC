/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:51:58 by egualand          #+#    #+#             */
/*   Updated: 2024/06/02 17:46:18 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_UTILS_HPP
# define SERVER_UTILS_HPP

# include <string>
# include <stdint.h>

#include "common_utils.hpp"

bool is_channel_prefix(const char c);
bool is_valid_channel_name(const std::string &name);
bool is_valid_channel_key(const std::string &key);
bool is_valid_channel_mode(const char mode);
bool is_valid_nickname(const std::string &name);
bool channel_mode_requires_param(const char mode, const bool status);
std::string hash(const std::string &s);
uint64_t ip_to_long(const std::string &ip);

#endif