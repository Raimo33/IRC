/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:10:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/30 16:30:36 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

using std::string;
using std::vector;
using std::ostringstream;

template <typename T>
string to_string(T value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

template<typename T>
string join(const vector<T>& elements, const string& delimiter)
{
	ostringstream oss;

	for (typename vector<T>::const_iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if (it != elements.begin())
			oss << delimiter;
		oss << *it;
	}
	return oss.str();
}
