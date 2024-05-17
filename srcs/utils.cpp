/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:54:09 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 16:58:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/utils.hpp"

bool	is_channel_prefix(const char c)
{
	return (c == '#' || c == '&'/* || c == '!' || c == '+' || c == '~' || c == '%' || c == '.'*/);
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
