/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:46:06 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 17:05:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_UTILS_HPP
#define COMMON_UTILS_HPP

#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

bool                     has_crlf(const std::string &s);
std::vector<std::string> split(const std::string &str, const std::string &delim);
std::string              get_next_token(std::string::iterator &it, const std::string::const_iterator &end, const char delim);
void                     configure_non_blocking(const int socket);

template <typename T>
std::string join(const std::vector<T> &elements, const std::string &delimiter);
template <typename T>
std::string to_string(T value);

#include "common_utils.tpp"

#endif