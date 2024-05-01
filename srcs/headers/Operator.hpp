/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:01:25 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 19:18:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

class Client;
class Channel;

#include <string>

using namespace std;

typedef enum channel_options
{
	MODE_I,
	MODE_T,
	MODE_K,
	MODE_O,
	MODE_L
}	e_channel_options;

class Operator : public Client
{
	public:
		Operator();
		Operator(const Operator &copy);
		~Operator();
		Operator &operator=(const Operator &copy);

		void	actionKick(const Client &client) const;
		void	actionInvite(const Client &client, const Channel &channel) const;
		string	actionTopic(const Channel &channel) const;
		void	actionMode(const Channel &channel, const channel_options &mode, const bool status) const;
	private:
		//
};


#endif