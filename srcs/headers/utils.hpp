/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:46:06 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 09:37:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <sstream>

using namespace std;

bool			is_channel_prefix(const char c);
bool			is_valid_channel_name(const string &name);
vector<string>	split(const string &s, char delim);
size_t			min(size_t a, size_t b);

#endif