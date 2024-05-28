/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:49:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/utils.hpp"
#include "irc/Constants.hpp"

#include <stdint.h>
#include <openssl/sha.h>
#include <iomanip>

using std::string;
using std::vector;
using std::stringstream;
using std::ostringstream;

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
	if (!isalpha(name[0]))
		return false;
	for (size_t i = 1; i < name.length(); i++)
	{
		char c = name[i];
		if (!isalnum(c) && c != '-' && !((c >= 91 && c <= 96) || c == '{' || c == '}' || c == '|'))
			return false;
	}
	return true;
}

bool	has_crlf(const string &s)
{
	uint32_t	len = s.length();

	return (s[len - 2] == '\r' && s[len - 1] == '\n');
}

bool	channel_mode_requires_param(const char mode)
{
	switch (mode)
	{
		case 'k':
		case 'l':
		case 'o':
			return true;
		default:
			return false;
	}
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

string to_hex(const unsigned char *bytes, size_t length)
{
	stringstream ss;
	for (size_t i = 0; i < length; ++i)
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
	return ss.str();
}

string	hash(const string &s)
{
	unsigned char	hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX		sha256;

	SHA256_Init(&sha256);
	SHA256_Update(&sha256, s.c_str(), s.size());
	SHA256_Final(hash, &sha256);

	return to_hex(hash, SHA256_DIGEST_LENGTH);
}