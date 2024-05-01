/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 00:12:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <map>
#include <string>

using namespace std;

class Channel;

class User
{
	public:
		User();
		User(const User &copy);
		~User();
		User	&operator=(const User &rhs);

		void	authenticate(const string &username, const string &password);
		void	setNickname(const string &nickname);
		void	joinChannel(const Channel &channel);
		void	sendMessage(const Channel &channel, const string &message) const;
	protected:
		map<string, Channel>	_channels;
		string					_nickname;
		string					_username;
		size_t					_pwd_hash;
	private:
		bool					_is_authenticated;
};

#endif