/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:46:06 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 21:07:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <sstream>

//TODO valutare se mettere in un namespace irc

bool						is_channel_prefix(const char c);
bool						is_valid_channel_name(const std::string &name);
bool						is_valid_nickname(const std::string &name);
std::vector<std::string>	split(const std::string &s, char delim);
size_t						min(size_t a, size_t b);

#endif