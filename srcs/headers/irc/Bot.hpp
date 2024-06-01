/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:43:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 15:39:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <string>

#include "ABot.hpp"

class Channel;

class Bot: public ABot
{
	public:
		explicit Bot(const std::string &nickname, const std::string &username, const std::string &realname);
		Bot(const Bot &copy);
		~Bot(void);

		void						run(void);

	private:
		void						joinChannel(const Channel &channel);
		void						leaveChannel(const Channel &channel);
};


#endif