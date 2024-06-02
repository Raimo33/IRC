/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 16:50:51 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.hpp"

#include <stdint.h>

using std::string;
using std::vector;
using std::stringstream;
using std::ostringstream;

string get_next_token(string::iterator &it, const string::const_iterator &end, const char delim)
{
	const string::iterator start = it;
	string token;

	while (it != end && *it == delim) // Skip leading delimiters
		it++;
	while (it != end && *it != delim) // Extract the token
		it++;
	token = string(start, it);
	while (it != end && *it == delim) // Skip trailing delimiters
		it++;
	return token;
}

bool has_crlf(const string &s)
{
	uint32_t len = s.length();

	if (len < 2)
		return false;
	return (s[len - 2] == '\r' && s[len - 1] == '\n');
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> tokens;
	size_t start = 0;
	size_t end = str.find(delim);

	while (end != string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + delim.length();
		end = str.find(delim, start);
	}
	tokens.push_back(str.substr(start, end));
	return tokens;
}