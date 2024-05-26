/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:46:06 by craimond          #+#    #+#             */
/*   Updated: 2024/05/26 17:19:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <sstream>

namespace irc
{
	bool						is_channel_prefix(const char c);
	bool						is_valid_channel_name(const std::string &name);
	bool						is_valid_channel_key(const std::string &key);
	bool						is_valid_nickname(const std::string &name);
	bool						has_crlf(const std::string &s);
	bool						channel_mode_requires_param(const char mode);
	std::vector<std::string>	split(const std::string &s, char delim);
	size_t						min(size_t a, size_t b);
	template <typename T>
	std::string					to_string(T value);
	template<typename T>
	std::string join(const std::vector<T>& elements, const std::string& delimiter);
}

#endif