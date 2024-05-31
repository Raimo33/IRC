/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:43:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 00:24:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

//TODO capire se il bot deve essere un client automatizzato simulato oppure parte del server. capire se usare thread o no

#include <string>

#include "ABot.hpp"

class Channel;

class Bot: public ABot
{
	public:
		explicit Bot(const std::string &nickname, const std::string &username, const std::string &realname);
		Bot(const Bot &copy);
		~Bot(void);

		Bot							&operator=(const Bot &copy);

		void						run(void);

	private:
		void						joinChannel(const Channel &channel);
		void						leaveChannel(const Channel &channel);
};


#endif