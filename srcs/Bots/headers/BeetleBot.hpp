/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:43:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 18:10:31 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <string>

#include "ABot.hpp"

class Channel;

class BeetleBot: public ABot
{
	public:
		explicit BeetleBot(const std::string &nickname, const std::string &username, const std::string &realname);
		BeetleBot(const BeetleBot &copy);
		~BeetleBot(void);

		void						run(void);

	private:
		void						joinChannel(const Channel &channel);
		void						leaveChannel(const Channel &channel);
};


#endif