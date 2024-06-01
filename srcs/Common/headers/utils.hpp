/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:46:06 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 19:31:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <sstream>
# include <stdint.h>

bool						is_channel_prefix(const char c);
bool						is_valid_channel_name(const std::string &name);
bool						is_valid_channel_key(const std::string &key);
bool						is_valid_channel_mode(const char mode);
bool						is_valid_nickname(const std::string &name);
bool						has_crlf(const std::string &s);
bool						channel_mode_requires_param(const char mode, const bool status);
std::vector<std::string>	split(const std::string &str, const std::string &delim);
std::string					hash(const std::string &s);
uint64_t					ip_to_long(const std::string& ip);

template <typename T>
std::string					join(const std::vector<T>& elements, const std::string& delimiter);
template<typename T>
std::string					to_string(T value);


#include "utils.tpp"

#endif