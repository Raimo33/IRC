/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:18:40 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 12:56:18 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <map>
# include <string>
# include "Channel.hpp"

using namespace std;

class Message;

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
		void	sendMessage(const Channel &channel, const Message *msg) const;
		
		string	getNickname() const;
	protected:
		map<string, const Channel *>	_channels; // {channel_name, channel}
		string							_nickname;
		string							_username;
		size_t							_pwd_hash;
	private:
		bool							_is_authenticated; //true se l'utente ha effettuato il login, serve per fargli fare la registrazione la prima volta
};

#endif