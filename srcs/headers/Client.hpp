/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 19:31:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <map>
#include <string>

using namespace std;

class Channel;

class Client
{
	public:
		Client();
		Client(const Client &copy);
		~Client();
		Client &operator=(const Client &copy);
		
	
	protected:
		map<string, Channel>	_channels;
		string					_nickname;
		string					_username;
		size_t					_pwd_hash;
		//
};

#endif