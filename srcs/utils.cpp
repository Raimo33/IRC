/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 09:37:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/utils.hpp"
#include "headers/Channel.hpp"

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
