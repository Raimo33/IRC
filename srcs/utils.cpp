/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/05/24 17:15:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/utils.hpp"
#include "irc/Constants.hpp"

#include <stdint.h>

using std::string;
using std::vector;
using std::stringstream;
using std::ostringstream;

namespace irc
{
	bool	is_channel_prefix(const char c)
	{
		return (c == '#' || c == '&'/* || c == '!' || c == '+' || c == '~' || c == '%' || c == '.'*/);
	}

	bool	is_valid_channel_name(const string &name)
	{
		if (name.empty() || !is_channel_prefix(name[0]) || name.length() > MAX_CHANNEL_NAME_LEN)
			return false;
		return true;
	}

	bool	is_valid_channel_key(const string &key)
	{
		if (key.empty() || key.length() > MAX_CHANNEL_KEY_LEN)
			return false;
		return true;
	}

	bool	is_valid_nickname(const string &name)
	{
		if (name.empty() || name.length() > MAX_NICKNAME_LEN)
			return false;
		if (isalpha(name[0]) == false)
			return false;
		for (size_t i = 1; i < name.length(); i++)
		{
			char c = name[i];
			if (isalnum(c) == false && c != '-' && !((c >= 91 && c <= 96) || c == '{' || c == '}' || c == '|'))
				return false;
		}
		return true;
	}

	bool	has_crlf(const string &s)
	{
		uint32_t	len = s.length();

		return (s[len - 2] == '\r' && s[len - 1] == '\n');
	}

	vector<string>	split(const string &s, char delim)
	{
		vector<string>	result;
		stringstream	ss(s);
		string			item;

		while (getline(ss, item, delim))
			result.push_back(item);
		return result;
	}

	size_t min(size_t a, size_t b)
	{
		return (a < b ? a : b);
	}


	template <typename T>
	string to_string(T value)
	{
		ostringstream oss;
		oss << value;
		return oss.str();
	}

	template <>
    std::string to_string<uint16_t>(uint16_t value)
	{
        return irc::to_string(value);
    }

	template <>
	std::string to_string<uint8_t>(uint8_t value)
	{
		return irc::to_string(value);
	}

	template<typename T>
	std::string join(const std::vector<T>& elements, const std::string& delimiter)
	{
		std::ostringstream oss;

		for (typename std::vector<T>::const_iterator it = elements.begin(); it != elements.end(); ++it)
		{
			if (it != elements.begin())
				oss << delimiter;
			oss << *it;
		}
		return oss.str();
	}

	template <>
	std::string join<std::string>(const std::vector<std::string>& elements, const std::string& delimiter)
	{
		return irc::join<std::string>(elements, delimiter);
	}
}