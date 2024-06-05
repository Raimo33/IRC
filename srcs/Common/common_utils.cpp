/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 14:43:22 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common_utils.hpp"
#include "common_constants.hpp"
#include "system_calls.hpp"

#include <stdint.h>

using std::ostringstream;
using std::string;
using std::stringstream;
using std::vector;

string get_next_token(string::iterator &it, const string::const_iterator &end, const char delim)
{
	const string::iterator start = it;
	string				   token;

	while (it != end && *it == delim) // Skip leading delimiters
		it++;
	while (it != end && *it != delim) // Extract the token
		it++;
	token = string(start, it);
	while (it != end && *it == delim) // Skip trailing delimiters
		it++;
	return token;
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> tokens;
	size_t		   start = 0;
	size_t		   end = str.find(delim);

	while (end != string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + delim.length();
		end = str.find(delim, start);
	}
	tokens.push_back(str.substr(start));
	return tokens;
}

void configure_non_blocking(const int socket)
{
	int flags;

	uint32_t buf_size = BUFFER_SIZE - 1;
	flags = fcntl_p(socket, F_GETFL);
	fcntl_p(socket, F_SETFL, flags | O_NONBLOCK);
	setsockopt_p(socket, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size));
}