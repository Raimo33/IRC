/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:49:59 by egualand          #+#    #+#             */
/*   Updated: 2024/07/10 14:37:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_utils.hpp"
#include "server_constants.hpp"

#include <cstring>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using std::string;
using std::stringstream;

bool is_channel_prefix(const char c)
{
	return (c == '#' || c == '&' /* || c == '!' || c == '+' || c == '~' || c == '%' || c == '.'*/);
}

bool is_valid_channel_name(const string &name)
{
	if (name.empty() || !is_channel_prefix(name[0]) || name.length() > MAX_CHANNEL_NAME_LEN)
		return false;
	return true;
}

bool is_valid_channel_key(const string &key)
{
	if (key.empty() || key.length() > MAX_CHANNEL_KEY_LEN)
		return false;
	return true;
}

bool is_valid_channel_mode(const char mode)
{
	return (std::strchr(SUPPORTED_CHANNEL_MODES, mode) != NULL);
}

bool is_valid_nickname(const string &name)
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

bool channel_mode_requires_param(const char mode, const bool status)
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

string to_hex(const unsigned char *bytes, size_t length)
{
	stringstream ss;
	for (size_t i = 0; i < length; ++i)
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
	return ss.str();
}

string hash(const string &s)
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

uint64_t ip_to_long(const std::string &ip)
{
	struct sockaddr_in sa;

	inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
	return ntohl(sa.sin_addr.s_addr);
}

bool	is_upper_finnish(const char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= '{' && c <= '}'));
}

string to_lower_finnish(const string &s)
{
	size_t	len = s.length();
	string	res = s;

	for (size_t i = 0; i < len; i++)
	{
		if (is_upper_finnish(res[i]))
			res[i] += 32;
	}
	return res;
}