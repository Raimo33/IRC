/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBot.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:43:07 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 15:45:33 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <string>

#include "ABot.hpp"

class BeetleBot: public ABot
{
	public:
		explicit BeetleBot(const std::string &nickname, const std::string &username, const std::string &realname);
		BeetleBot(const BeetleBot &copy);
		~BeetleBot(void);

		const std::map<std::string, AAction *>	initActions(void) const;
};


#endif