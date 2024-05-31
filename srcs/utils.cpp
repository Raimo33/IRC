/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 19:24:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/utils.hpp"
#include "irc/Constants.hpp"

#include <stdint.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iomanip>
#include <cstring>
#include <arpa/inet.h>

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

bool	is_valid_channel_mode(const char mode)
{
	return (std::strchr(SUPPORTED_CHANNEL_MODES, mode) != NULL);
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

	if (len < 2)
		return false;
	return (s[len - 2] == '\r' && s[len - 1] == '\n');
}

bool	channel_mode_requires_param(const char mode, const bool status)
{
	switch (mode)
	{
		case 'k':
		case 'l':
			return status;
		case 'o':
			return true;
		default:
			return false;
	}
}

vector<string>	split(const string &str, const string &delim)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delim);

	while (end != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + delim.length();
		end = str.find(delim, start);
	}
	tokens.push_back(str.substr(start, end));
	return tokens;
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
	unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX *context = EVP_MD_CTX_new();

    EVP_DigestInit_ex(context, EVP_sha256(), NULL);
	EVP_DigestUpdate(context, s.c_str(), s.size());
	EVP_DigestFinal_ex(context, hash, &lengthOfHash);
    EVP_MD_CTX_free(context);

    return to_hex(hash, lengthOfHash);
}

uint64_t	ip_to_long(const std::string& ip)
{
    struct sockaddr_in sa;

    inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
    return ntohl(sa.sin_addr.s_addr);
}